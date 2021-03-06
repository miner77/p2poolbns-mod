//#include "main.h"
//#include "addrman.h"
//#include "alert.h"
//#include "chainparams.h"
//#include "checkpoints.h"
//#include "checkqueue.h"
//#include "init.h"
//#include "net.h"
//#include "ui_interface.h"
//#include "util.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;
using namespace boost;


int static generateMTRandom(unsigned int s, int range)
{
    random::mt19937 gen(s);
    random::uniform_int_distribution<> dist(1, range);
    return dist(gen);
}

static const int64_t COIN = 100000000;
static const int64_t  nStartSubsidy = 2300 * COIN;
static const int64_t  nMinSubsidy = 10 * COIN;

int64_t  static GetBlockValue(int nHeight, int64_t nFees, uint256 prevHash)
{

    std::string cseed_str = prevHash.ToString().substr(25,7);
    const char* cseed = cseed_str.c_str();
    long seed = hex2long(cseed);
    int rand = generateMTRandom(seed, 1024);
    if (fDebug) { printf(">> nHeight = %d, rand = %d\n", nHeight, rand); }

    int64_t nSubsidy = nStartSubsidy;

    //see if there's a bonus
    //a range of 256 possible values; (256 / 1024) = 1:4
    if(rand > 0 && rand < 257)
    {
        nSubsidy *= 2;
    }
    //a range of 64 possible values; (64 / 1024) = 1:16
    else if(rand > 300 && rand < 365)
    {
        nSubsidy *= 10;
    }
    //one possible value; (1 / 1024) = 1:1024
    else if(rand == 512)
    {
        nSubsidy *= 250;
 }

    if(nHeight == 1) {
        //add insta-mine code (hardcode the value to avoid any possibility of a bonus being added
        nSubsidy = 25000000 * COIN;
    }
        //the first 100,000 blocks are a special case
    else if(nHeight <= 50000) {
        nSubsidy *= 1.25; //bump up the first 100,000 blocks
    } else {
        // Mining phase: Subsidy is cut in half every SubsidyHalvingInterval
        nSubsidy >>= (nHeight / 300000);
    }

    // Inflation phase: Subsidy reaches minimum subsidy
    // Network is rewarded for transaction processing with transaction fees and
    // the inflationary subsidy
    if (nSubsidy < nMinSubsidy)
    {
        nSubsidy = nMinSubsidy;
    }

    //commented out fees, I think this should do what we want
    return nSubsidy; //+ nFees;
}

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(bonuscoin_subsidy)
{
    def("GetBlockValue", GetBlockValue);
}
