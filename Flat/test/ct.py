#!/usr/bin/env python

from re import sub
from sys import argv,exit
from os import system,getenv
import json

debug_level = 0
torun = argv[1]
output = 'testskim.root'
if len(argv)>2:
    debug_level = int(argv[2])
    if len(argv)>3:
        output = argv[3]

argv = []

import ROOT as root
from PandaCore.Utils.load import *
from PandaAnalysis.Flat.analysis import *
import PandaAnalysis.T3.job_utilities as utils

Load('PandaAnalysisFlat')

a = analysis('ct', ak=True, ak8=True, puppiJets=True)
a.processType = root.pa.kTop

a.inpath = torun
a.outpath = 'testskim.root'
a.datapath = getenv('CMSSW_BASE') + '/src/PandaAnalysis/data/'

a.isData = False
utils.set_year(a, 2016)
#utils.set_year(a, 2017)

skimmer = root.pa.CTAnalyzer(a, debug_level)

#skimmer.firstEvent=0
skimmer.lastEvent=100

skimmer.Run()
skimmer.Terminate()
