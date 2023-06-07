// Copyright 2022, Thomas Britton
// Subject to the terms in the LICENSE file found in the top-level directory.
//

#pragma once

#include <random>

#include <services/io/podio/JFactoryPodioT.h>
#include <services/geometry/dd4hep/JDD4hep_service.h>
#include <algorithms/calorimetry/CalorimeterClusterRecoCoG.h>
#include <services/log/Log_service.h>
#include <extensions/spdlog/SpdlogExtensions.h>



class Cluster_factory_FAEMCalClusters : public eicrecon::JFactoryPodioT<edm4eic::Cluster>, CalorimeterClusterRecoCoG {

public:
    //------------------------------------------
    // Constructor
    Cluster_factory_FAEMCalClusters(){
        SetTag("FAEMCalClusters");
        m_log = japp->GetService<Log_service>()->logger(GetTag());
    }

    //------------------------------------------
    // Init
    void Init() override{
        auto app = GetApplication();
        //-------- Configuration Parameters ------------
        m_input_simhit_tag="FAEMCalHits";
        m_input_protoclust_tag="FAEMCalProtoClusters";

        m_sampFrac=1.0;//{this, "samplingFraction", 1.0};
        m_logWeightBase=6.2;//{this, "logWeightBase", 3.6};
        m_depthCorrection=0.0;//{this, "depthCorrection", 0.0};
        m_energyWeight="log";//{this, "energyWeight", "log"};
        m_moduleDimZName="";//{this, "moduleDimZName", ""};
        // Constrain the cluster position eta to be within
        // the eta of the contributing hits. This is useful to avoid edge effects
        // for endcaps.
        m_enableEtaBounds=false;//{this, "enableEtaBounds", false};

        app->SetDefaultParameter("BEMC:FAEMCalClusters:input_protoclust_tag", m_input_protoclust_tag, "Name of input collection to use");
        app->SetDefaultParameter("BEMC:FAEMCalClusters:samplingFraction",             m_sampFrac);
        app->SetDefaultParameter("BEMC:FAEMCalClusters:logWeightBase",  m_logWeightBase);
        app->SetDefaultParameter("BEMC:FAEMCalClusters:depthCorrection",     m_depthCorrection);
        app->SetDefaultParameter("BEMC:FAEMCalClusters:input_simhit_tag", m_input_simhit_tag);
        app->SetDefaultParameter("BEMC:FAEMCalClusters:energyWeight",   m_energyWeight);
        app->SetDefaultParameter("BEMC:FAEMCalClusters:moduleDimZName",   m_moduleDimZName);
        app->SetDefaultParameter("BEMC:FAEMCalClusters:enableEtaBounds",   m_enableEtaBounds);

        m_geoSvc = app->template GetService<JDD4hep_service>();

        AlgorithmInit(m_log);
    }

    //------------------------------------------
    // ChangeRun
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override{
        AlgorithmChangeRun();
    }

    //------------------------------------------
    // Process
    void Process(const std::shared_ptr<const JEvent> &event) override{


        // Prefill inputs
        m_inputSimhits=event->Get<edm4hep::SimCalorimeterHit>(m_input_simhit_tag);
        m_inputProto=event->Get<edm4eic::ProtoCluster>(m_input_protoclust_tag);

        // Call Process for generic algorithm
        AlgorithmProcess();

        // Hand owner of algorithm objects over to JANA
        Set(m_outputClusters);
        event->Insert(m_outputAssociations, "FAEMCalClusterAssociations");
        m_outputClusters.clear(); // not really needed, but better to not leave dangling pointers around
        m_outputAssociations.clear();
    }
};
