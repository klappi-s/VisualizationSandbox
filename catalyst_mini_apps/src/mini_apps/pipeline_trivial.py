# script-version: 2.0
import os
import sys
import time
import argparse

import paraview
import paraview.catalyst
from paraview import catalyst
from paraview import print_info
from paraview import servermanager
from paraview import servermanager as sm

from paraview.simple import *
from paraview.simple import CreateExtractor

# Add the directory containing this script to the path
sys.path.append(os.path.dirname(__file__))


def _log(msg):
    """Logs a message from the Catalyst script."""

    controller = sm.vtkProcessModule.GetProcessModule().GetGlobalController()
    rank = 0
    if controller:
        rank = controller.GetLocalProcessId()
        msg =  "[rank " + str(rank) + "]:  " + msg
    

    print( msg  )


def print_proxy_overview():
    """Prints a list of all available 'sources' proxies."""
    pm = servermanager.ProxyManager()
    _log("Available 'sources' proxies:")
    for (proxy_name, _), proxy_id in pm.GetProxiesInGroup("sources").items():
        proxy = pm.GetProxy("sources", proxy_name)
        _log(f" - Proxy Name: {proxy_name} (Class: {proxy.GetXMLName()})")


def create_VTPD_extractor(name, data_source, freq=1):
    """Creates a VTPD extractor for partitioned data."""
    extractor = CreateExtractor('VTPD', data_source, registrationName=f'VTPD_{name}')
    extractor.Trigger.Frequency = freq
    extractor.Writer.FileName = f'ippl_{name}_{{timestep:06d}}.vtpd'
    return extractor

# --- Global Scope Setup ---

# Parse args passed from C++
_arg_list = paraview.catalyst.get_args()
_parser = argparse.ArgumentParser()
_parser.add_argument("--channel_names", nargs="*", default=["uniform"], help="Channel names")
_parser.add_argument("--VTKextracts", default="ON", help="(ignored)")
_parsed = _parser.parse_args(_arg_list)

# Configure Catalyst run options
options = catalyst.Options()
options.GlobalTrigger = 'Time Step'
options.EnableCatalystLive = 1
options.CatalystLiveTrigger = 'Time Step'
options.ExtractsOutputDirectory = 'data_vtk_extracts'
os.makedirs(options.ExtractsOutputDirectory, exist_ok=True)

_log(f"pipeline_trivial: channels={_parsed.channel_names}")

# Global dictionaries to store proxies and filters
extractors = {}
_sources = {}
_live_filters = {} # For the MergeBlocks filters

# Find proxies and set up pipelines in the global scope
pm = servermanager.ProxyManager()
for cname in _parsed.channel_names:
    # 1. Find the raw Conduit proxy that Catalyst creates for this channel
    proxy = pm.GetProxy("sources", cname) 
    if not proxy:
        _log(f"WARNING: Could not find auto-generated proxy for channel '{cname}'")
        continue

    _log(f"Found auto-generated source proxy: {cname} ({proxy.GetXMLName()})")
    _sources[cname] = proxy
    
    # 2. Setup File Extraction (attaches to the raw proxy)
    if _parsed.VTKextracts == "ON":
        _log(f"Attaching VTPD extractor to raw proxy '{cname}'")
        extractors[cname] = create_VTPD_extractor(cname, proxy, 1)

    # 3. Setup Live Visualization (create filter, but don't show it yet)
    if options.EnableCatalystLive:
        _log(f"Creating MergeBlocks filter for Live-Vis on '{cname}'")
        merged = MergeBlocks(Input=proxy)
        merged.MergePartitionsOnly = 1
        _live_filters[cname] = merged # Store the filter for later
        
        # CRITICAL: DO NOT call Show(merged) here.
        # This causes a premature update on an empty pipeline.

print_proxy_overview()

# --- Catalyst API Functions ---

def catalyst_initialize():
    _log("catalyst_initialize()")


def catalyst_execute(info):
    _log(f"catalyst_execute(cycle={info.cycle}, time={info.time})")
    
    global _parsed 
    global _sources
    global extractors
    global _live_filters
    
    pm = servermanager.ProxyManager()

    # Loop over all registered channels
    for name in _sources.keys():
        
        # 1. Get the proxy for this channel
        #    The Catalyst bridge has *already* updated this proxy with new data.
        proxy = pm.GetProxy("sources", name)
        if not proxy:
             _log(f"WARNING: proxy '{name}' not found in step {info.cycle}")
             continue

        # 2. Manually update the proxy's pipeline.
        #    This is necessary to make the new data available
        #    to downstream filters (like the extractors).
        proxy.UpdatePipeline()
        proxy.UpdateVTKObjects()

        # 3. FIX for Live Vis: Re-set the MergeBlocks filter's input.
        #    This forces it to connect to the new, complete partitioned
        #    dataset instead of a stale, old (rank 0) proxy.
        if name in _live_filters:
            live_filter = _live_filters[name]
            live_filter.Input = proxy
            
            # 4. On the very first timestep, call Show()
            #    This makes the filter visible in the ParaView GUI.
            if info.cycle == 0 and options.EnableCatalystLive:
                Show(live_filter)

    if options.EnableCatalystLive:
        time.sleep(0.5)


def catalyst_finalize():
    _log("catalyst_finalize()")