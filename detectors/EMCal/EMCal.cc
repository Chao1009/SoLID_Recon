// Copyright 2023, Chao Peng
// Subject to the terms in the LICENSE file found in the top-level directory.
//
//

#include <JANA/JApplication.h>
#include <JANA/JFactoryGenerator.h>

#include "RawCalorimeterHit_factory_FAEMCalRawHits.h"
#include "CalorimeterHit_factory_FAEMCalRecHits.h"
#include "ProtoCluster_factory_FAEMCalProtoClusters.h"
#include "Cluster_factory_FAEMCalClusters.h"


extern "C" {
    void InitPlugin(JApplication *app) {
        InitJANAPlugin(app);

        app->Add(new JFactoryGeneratorT<RawCalorimeterHit_factory_FAEMCalRawHits>());
        app->Add(new JFactoryGeneratorT<CalorimeterHit_factory_FAEMCalRecHits>());
        app->Add(new JFactoryGeneratorT<ProtoCluster_factory_FAEMCalProtoClusters>());
        app->Add(new JFactoryGeneratorT<Cluster_factory_FAEMCalClusters>());
    }
}
