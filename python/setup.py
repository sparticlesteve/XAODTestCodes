import sys

def setup_rc_batch():
    """Sets up RootCore libs and runs ROOT in batch mode"""
    # Save command line args
    argv = sys.argv
    # Force batch mode
    sys.argv = [argv[0], '-b']
    from ROOT import gROOT
    gROOT.Macro('$ROOTCOREDIR/scripts/load_packages.C')
    # Now restore command line args
    sys.argv = argv

def load_rc_libs(batch=False):
    """Sets up RootCore libs and optionally loads ROOT in batch mode"""
    # Save command line args to hide them from ROOT
    _argv = sys.argv
    sys.argv = [_argv[0]]
    # Force batch mode
    if batch: sys.argv.append('-b')
    from ROOT import gROOT
    gROOT.Macro('$ROOTCOREDIR/scripts/load_packages.C')
    # Now restore command line args
    sys.argv = _argv
