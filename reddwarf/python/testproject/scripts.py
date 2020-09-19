# -*- coding: utf-8 -*-
"""
Created on Sun Aug  2 12:52:36 2020

@author: eddeighton
"""



#code is in W:\WinPython\WPy64-3771\python-3.7.7.amd64\Lib\site-packages\ipykernel\eventloops.py


#C:\Users\eddeighton\AppData\Roaming\jupyter\runtime

import python_host
host = python_host.GetHost()
prog = host.getProgram()
root = prog.getRoot()

tnk = root.Tank()

torso = root.Torso()


root.RedTest()
root.DeepTest()
root.DumpEdFile().m_filePath( "w:/root/editor/src/data/testMeta.blu" )