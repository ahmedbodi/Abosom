// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2019-2020 The Abosom Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "chain.h"
#include "chainparams.h"
#include <reverse_iterator.h>
#include "validation.h"
#include "uint256.h"

#include <stdint.h>

static const int nCheckpointSpan = 500;

namespace Checkpoints {

    CBlockIndex* GetLastCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        for (const MapCheckpoints::value_type& i : reverse_iterate(checkpoints))
        {
            const uint256& hash = i.second;
            BlockMap::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindexBest = chainActive.Tip();
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;

        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        if(nHeight)
        {
            auto pindexSync = AutoSelectSyncCheckpoint();
            return nHeight > pindexSync->nHeight;
        }

        return true;
    }

} // namespace Checkpoints

