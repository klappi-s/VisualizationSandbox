# script-version: 2.0
# Minimal Catalyst 2.0 pipeline: create PVTrivialProducer(s) for incoming channels
# and update them each execute. Enables Live so you can connect from ParaView.

import os
import paraview
from paraview.simple import PVTrivialProducer, SaveData
import paraview.catalyst
from paraview import catalyst, print_info
from paraview.simple import *
from paraview import servermanager
from paraview.simple import CreateExtractor
import sys

import argparse


import paraview
from paraview.simple import *
# paraview.compatibility.major = 5
# paraview.compatibility.minor = 12
import paraview.catalyst
from paraview import catalyst

import paraview.simple as pvs
from paraview.simple import (
    LoadPlugin,
    CreateSteerableParameters,
    PVTrivialProducer,
    MergeBlocks, 
    Show
)
from paraview import servermanager
from paraview import print_info

import argparse
import sys
import time
import os

sys.path.append(os.path.dirname(__file__))






# globals
# _options = None
_parsed = None


def _log(msg):
    print_info(msg)



def print_proxy_overview():
    # _log("====Printing Proxy Overview  ===========")
    pm = servermanager.ProxyManager()
    _log("Available 'sources' proxies:")
    for (proxy_name, _), proxy_id in pm.GetProxiesInGroup("sources").items():
        proxy = pm.GetProxy("sources", proxy_name)
        _log(f" - ProxyPrint: {proxy})")
        _log(f" - Proxy Name: {proxy_name}")
        _log(f"   - XML Label: {proxy.GetXMLLabel()}")
        _log(f"   - XML Group: {proxy.GetXMLGroup()}")
        _log(f"   - Class Name: {proxy.GetXMLName()}")
        _log(f"   - Properties:")
        for prop_name in proxy.ListProperties():
            _log(f"     - {prop_name}")


def create_VTPD_extractor(name, object, fr = 1):
    vTPD = CreateExtractor('VTPD', object, registrationName='VTPD_'+ name)
    vTPD.Trigger.Frequency = fr
    vTPD.Writer.FileName = 'ippl_'+name+'_{timestep:06d}.vtpd'
    return vTPD
    




# Parse args passed from C++ (via conduit node)
_arg_list = paraview.catalyst.get_args()
_parser = argparse.ArgumentParser()
_parser.add_argument("--channel_names", nargs="*", default=["uniform"], help="Channel names to attach producers to")
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




extractors = {}
_vis_producers = {}
_sources = {}



for cname in _parsed.channel_names:
    _vis_producers[cname] = PVTrivialProducer(registrationName=cname)



if _parsed.VTKextracts == "ON" and _parsed.channel_names:
    for cname, reader in _vis_producers.items():
        extractors[cname] = create_VTPD_extractor(cname, reader, 1)



# if _parsed.VTKextracts == "ON" and _parsed.channel_names:
#     pm = servermanager.ProxyManager()
#     for cname in _parsed.channel_names:
#         # Find the source proxy that Catalyst automatically created for this channel.
#         proxy = pm.GetProxy("sources", cname) 
        
#         if proxy:
#             _log(f"Found auto-generated source proxy: {cname} ({proxy.GetXMLName()})")
#             _sources[cname] = proxy
#             # Attach the extractor to the PROXY, not a PVTrivialProducer
#             extractors[cname] = create_VTPD_extractor(cname, proxy, 1)
#         else:
#             _log(f"WARNING: Could not find auto-generated proxy for channel '{cname}'")


uniform_prod = PVTrivialProducer(registrationName="uniform")
vtpd_extract = create_VTPD_extractor("uniform", uniform_prod, 1)




print_proxy_overview()

def catalyst_initialize():
    _log("catalyst_initialize()")


def catalyst_execute(info):
    _log(f"catalyst_execute(cycle={info.cycle}, time={info.time})")
    # global uniform_prod
    # uniform_prod.UpdatePipeline()






    global _vis_producers
    global extractors 
    global _parsed 
    global _sources
    global extractors

    for name, prod in _vis_producers.items():
        prod.UpdateVTKObjects()
        prod.UpdatePipeline()
    # for name, prod in _sources.items():
        # prod.UpdatePipeline()
        # prod.UpdateVTKObjects()

    if options.EnableCatalystLive:
        time.sleep(0.5)


def catalyst_finalize():
    _log("catalyst_finalize()")











        # # Unconditional VTK extracts using SaveData; try .vtpd (partitioned dataset)
        # outdir = options.ExtractsOutputDirectory
        # fname = os.path.join(outdir, f"{name}_{info.cycle:06d}.vtpd")
        # try:
        #     SaveData(fname, proxy=_sources[name])
        #     _log(f"Wrote extract: {fname}")
        # except Exception as e:
        #     # fallback to .vti if vtpd fails
        #     fname_vti = os.path.join(outdir, f"{name}_{info.cycle:06d}.vti")
        #     try:
        #         SaveData(fname_vti, proxy=_sources[name])
        #         _log(f"Wrote extract: {fname_vti}")
        #     except Exception as ee:
        #         _log(f"Failed to write extract for '{name}': {ee}")
