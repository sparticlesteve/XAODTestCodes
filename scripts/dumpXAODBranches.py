#!/bin/env python

# Setup RootCore libs
from XAODTestCodes.setup import setup_rc_batch
setup_rc_batch()

import argparse
from ROOT import TFile

# Default xAOD file ($ROOTCORE_TEST_FILE)
default_input = '/afs/cern.ch/user/a/asgbase/patspace/xAODs/r6630/mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.recon.AOD.e3698_s2608_s2183_r6630_tid05352803_00/AOD.05352803._000242.pool.root.1'

# Command line processing
parser = argparse.ArgumentParser('dumpXAODBranches')
parser.add_argument('-i', '--input', default=default_input,
                    help='input xAOD file')
args = parser.parse_args()

f = TFile(args.input)
t = f.Get('CollectionTree')

branches = t.GetListOfBranches()
bnames = sorted([b.GetName() for b in branches])
for b in bnames:
    print b
