"""
    Standard environment setup for RC in python/ipython.
    This script will load the RC libs and import some common modules.
"""

import sys
from XAODTestCodes.setup import load_rc_libs
load_rc_libs()

import ROOT
from ROOT import SH, EL, xAOD
