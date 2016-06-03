// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparamsbase.h"

#include "tinyformat.h"
#include "util.h"

#include <assert.h>

const std::string CBaseChainParams::MAIN = "main";
const std::string CBaseChainParams::TESTNET = "test";
const std::string CBaseChainParams::REGTEST = "regtest";
const std::string CBaseChainParams::SEGNET = "segnet";
const std::string CBaseChainParams::BSAFENET = "bsafenet";

void AppendParamsHelpMessages(std::string& strUsage, bool debugHelp)
{
    strUsage += HelpMessageGroup(_("Chain selection options:"));
    strUsage += HelpMessageOpt("-testnet", _("Use the test chain"));
    if (debugHelp) {
        strUsage += HelpMessageOpt("-regtest", "Enter regression test mode, which uses a special chain in which blocks can be solved instantly. "
                                   "This is intended for regression testing tools and app development.");
        strUsage += HelpMessageOpt("-segnet", "Enter segregated witness test mode. ");
        strUsage += HelpMessageOpt("-bsafenet", "Enter segregated witness test mode on bsafenet. ");
    }
}

/**
 * Main network
 */
class CBaseMainParams : public CBaseChainParams
{
public:
    CBaseMainParams()
    {
        nRPCPort = 8332;
    }
};
static CBaseMainParams mainParams;

/**
 * Testnet (v3)
 */
class CBaseTestNetParams : public CBaseChainParams
{
public:
    CBaseTestNetParams()
    {
        nRPCPort = 18332;
        strDataDir = "testnet3";
    }
};
static CBaseTestNetParams testNetParams;

/**
 * Segnet
 */
class CBaseSegNetParams : public CBaseChainParams
{
public:
    CBaseSegNetParams()
    {
        nRPCPort = 28902;
        strDataDir = "segnet4";
    }
};
static CBaseSegNetParams segNetParams;

/**
 * bsafenet
 */
class CBaseSafeNetParams : public CBaseChainParams
{
public:
    CBaseSafeNetParams()
    {
        nRPCPort = 34822;
        strDataDir = "bsafenet";
    }
};
static CBaseSafeNetParams bSafeParams;

/*
 * Regression test
 */
class CBaseRegTestParams : public CBaseChainParams
{
public:
    CBaseRegTestParams()
    {
        nRPCPort = 18332;
        strDataDir = "regtest";
    }
};
static CBaseRegTestParams regTestParams;

static CBaseChainParams* pCurrentBaseParams = 0;

const CBaseChainParams& BaseParams()
{
    assert(pCurrentBaseParams);
    return *pCurrentBaseParams;
}

CBaseChainParams& BaseParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
        return testNetParams;
    else if (chain == CBaseChainParams::SEGNET)
        return segNetParams;
    else if (chain == CBaseChainParams::REGTEST)
        return regTestParams;
    else if (chain == CBaseChainParams::BSAFENET)
        return bSafeParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectBaseParams(const std::string& chain)
{
    pCurrentBaseParams = &BaseParams(chain);
}

std::string ChainNameFromCommandLine()
{
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);
    bool fSegNet = GetBoolArg("-segnet", false);
    bool fbsafeNet = GetBoolArg("-bsafenet", false);

    if ((int)fRegTest + (int)fTestNet + (int)fSegNet + (int)fbsafeNet > 1)
        throw std::runtime_error("Invalid combination of -regtest, -testnet, -segnet, -bsafenet.");
    if (fRegTest)
        return CBaseChainParams::REGTEST;
    if (fTestNet)
        return CBaseChainParams::TESTNET;
    if (fSegNet)
        return CBaseChainParams::SEGNET;
    if (fbsafeNet)
        return CBaseChainParams::BSAFENET;
    return CBaseChainParams::MAIN;
}

bool AreBaseParamsConfigured()
{
    return pCurrentBaseParams != NULL;
}
