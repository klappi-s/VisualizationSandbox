# script-version: 2.0
import os
import sys
import time
import argparse
# paraview.compatibility.major = 5
# paraview.compatibility.minor = 12
import paraview
import paraview.catalyst
from paraview import catalyst
from paraview import print_info
from paraview import servermanager

from paraview.simple import *
from paraview.simple import CreateExtractor
from paraview.simple import (
    PVTrivialProducer,
)
sys.path.append(os.path.dirname(__file__))



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
    

_parsed = None
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



_vis_producers = {}
for cname in _parsed.channel_names:
    _vis_producers[cname] = PVTrivialProducer(registrationName=cname)

extractors = {}
if _parsed.VTKextracts == "ON" and _parsed.channel_names:
    for cname, reader in _vis_producers.items():
        extractors[cname] = create_VTPD_extractor(cname, reader, 1)


print_proxy_overview()

def catalyst_initialize():
    _log("catalyst_initialize()")


def catalyst_execute(info):
    _log(f"catalyst_execute(cycle={info.cycle}, time={info.time})")

    global _vis_producers

    for name, prod in _vis_producers.items():
        prod.UpdateVTKObjects()
        prod.UpdatePipeline()

    if options.EnableCatalystLive:
        time.sleep(0.5)


def catalyst_finalize():
    _log("catalyst_finalize()")

