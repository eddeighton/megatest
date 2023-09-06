
#  Copyright (c) Deighton Systems Limited. 2022. All Rights Reserved.
#  Author: Edward Deighton
#  License: Please see license.txt in the project root folder.
#
#  Use and copying of this software and preparation of derivative works
#  based upon this software are permitted. Any copy of this software or
#  of any derivative work must include the above copyright notice, this
#  paragraph and the one after it.  Any distribution of this software or
#  derivative works must comply with all applicable laws.
#
#  This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
#  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
#  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
#  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
#  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
#  OF THE POSSIBILITY OF SUCH DAMAGES.


import sys
import os
import argparse

# grab the mega structure environment
WORKSPACE_ROOT_PATH = os.environ['WORKSPACE_ROOT_PATH']
BUILD_PATH = os.environ['BUILD_PATH']
CFG_LINK = os.environ['CFG_LINK']
CFG_TYPE = os.environ['CFG_TYPE']
CFG_TUPLE = os.environ['CFG_TUPLE']
MEGA_BIN =  BUILD_PATH + '/' + CFG_TUPLE + "/mega/install/bin" 
sys.path.append( MEGA_BIN )

# load the megastructure python component
import megastructure
mega = megastructure
megastructure.run_one()
megastructure.run_one()

# setup command line arguments handling
parser = argparse.ArgumentParser(
    prog = "Basic Construct",
    description = "Simple utility for testing megastructure" )
parser.add_argument( "-m", "--mpo", dest="mpo", help="Specify the Machine Process Owner ( MPO )" )
parser.add_argument( "-t", "--toasters", dest="toasters", help="Create some toasters" )
parser.add_argument( "-d", "--dump", dest="dump", help="Dump root", action="store_true" )

args = parser.parse_args()

print( "Attempting to acquire mpo: ", args.mpo )

# attempt to get the mpo root
mpo = mega.getMPO( str( args.mpo ) )
print( "Acquired mpo: ", mpo )

root = mpo.getRoot()
print( "Acquired root: ", root )

def createToasters( number ):
    for i in range( int( number )  ):
        toaster = root.Parent_Toaster.Toaster()
        print( "Created toaster: ", toaster )

if args.toasters:
    createToasters( args.toasters )

if args.dump:
    print( root.dump() )

mega.cycle()
megastructure.run_one()


#def main( *args ):
#    print( "Got arguments: ", args )#

#if __name__ == '__main__':
#    name_Script, *script_args = sys.argv
#    print( "Name of the script: ", name_Script )
#    main(*script_args) #Send list of arguments
