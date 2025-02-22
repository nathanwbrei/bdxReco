cmake_minimum_required(VERSION 3.13)
project(bdxreco)

set(CMAKE_CXX_STANDARD 14)

# Enable -fPIC for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions(-DHAVE_CCDB)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(src/external)
include_directories(src/libraries)
include_directories(.)
include_directories(/usr/local/include/)

set(SITE /site/12gev_phys/2.3/Linux_RedHat7.6-x86_64-gcc4.8.5)

#set(ENV{CCDB_HOME} /Users/nbrei/src/ccdb)
#set(ENV{CLHEP_BASE_DIR} /usr/local/Cellar/clhep/2.4.1.2)
#set(ENV{EVIO} /Users/nbrei/src/evio)
#set(ENV{XERCESCROOT} /usr/local/Cellar/xerces-c/3.2.2)
#set(ENV{BDXRECO_ROOT} /Users/nbrei/src/bdxReco)

if (DEFINED ENV{XERCESCROOT})
    include_directories($ENV{XERCESCROOT}/include)
    link_libraries($ENV{XERCESCROOT}/lib/libxerces-c.so)
    MESSAGE(STATUS "Using XERCESCROOT = " $ENV{XERCESCROOT})
else()
    include_directories(${SITE}/xercesc/3.2.2/include)
    link_libraries(${SITE}/xercesc/3.2.2/lib/libxerces-c.so)
    MESSAGE(WARNING "Using defaulted XERCESCROOT")
endif()

if (DEFINED ENV{CCDB_HOME})
    include_directories($ENV{CCDB_HOME}/include)
    link_libraries($ENV{CCDB_HOME}/lib/libccdb.so)
    MESSAGE(STATUS "Using CCDB_HOME = " $ENV{CCDB_HOME})
else()
    include_directories(${SITE}/ccdb/ccdb-1.06.02/include)
    link_libraries(${SITE}/ccdb/ccdb-1.06.02/lib/libccdb.so)
    MESSAGE(WARNING "Using defaulted CCDB_HOME")
endif()

if (DEFINED ENV{EVIO})
    include_directories($ENV{EVIO}/src/libsrc)
    include_directories($ENV{EVIO}/src/libsrc++)
        # N.B. The order of libevio.a and libevioxx.a matters, since the latter depends on the former
    link_libraries($ENV{EVIO}/lib/libevioxx.a)
    link_libraries($ENV{EVIO}/lib/libevio.a)
    link_libraries(expat)
    link_libraries(z)
    link_libraries(pthread)
    MESSAGE(STATUS "Using EVIO = " $ENV{EVIO})
else()
    include_directories(${SITE}/evio/5.1/src/libsrc)
    include_directories(${SITE}/evio/5.1/src/libsrc++)
    link_libraries(${SITE}/evio/5.1/lib/libevio.a)
    link_libraries(${SITE}/evio/5.1/lib/libevioxx.a)
    link_libraries(expat)
    link_libraries(z)
    link_libraries(pthread)
    MESSAGE(WARNING "Using defaulted EVIO")
endif()

if (DEFINED ENV{CLHEP_BASE_DIR})
    include_directories($ENV{CLHEP_BASE_DIR}/include)
    link_libraries($ENV{CLHEP_BASE_DIR}/lib/libCLHEP.a)
    MESSAGE(STATUS "Using CLHEP_BASE_DIR = " $ENV{CLHEP_BASE_DIR})
else()
    include_directories(${SITE}/clhep/2.4.0.4/include)
    link_libraries(${SITE}/clhep/2.4.0.4/lib/libCLHEP.a)
    MESSAGE(WARNING "Using defaulted CLHEP")
endif()

add_library(tridas STATIC
        src/external/TRIDAS/chrono_io.hpp
        src/external/TRIDAS/f_dataformat_bdx.hpp
        src/external/TRIDAS/pt_file_analyser.cpp
        src/external/TRIDAS/ptfile_reader.cc
        src/external/TRIDAS/ptfile_reader.hpp
        src/external/TRIDAS/s_dataformat_bdx.hpp
        src/external/TRIDAS/tridas_dataformat.hpp
        )

add_library(evioReader STATIC
        src/external/evioReader/common_std.h
        src/external/evioReader/EvioBankIndex.cc
        src/external/evioReader/EvioBankIndex.h
        src/external/evioReader/EvioCompositeDecoder.cc
        src/external/evioReader/EvioCompositeDecoder.h
        src/external/evioReader/EvioDataBank.cc
        src/external/evioReader/EvioDataBank.h
        src/external/evioReader/EvioDataDescriptor.cc
        src/external/evioReader/EvioDataDescriptor.h
        src/external/evioReader/EvioDataEvent.cc
        src/external/evioReader/EvioFileReader.cc
        src/external/evioReader/EvioFileReader.h
        )


add_library(jana1 STATIC
        #src/external/jana_0.7.7p1/src/JANA/force_link.cc
        src/external/jana_0.7.7p1/src/JANA/jana_config.h
        src/external/jana_0.7.7p1/src/JANA/JApplication.cc
        src/external/jana_0.7.7p1/src/JANA/JApplication.h
        src/external/jana_0.7.7p1/src/JANA/JCalibration.cc
        src/external/jana_0.7.7p1/src/JANA/JCalibration.h
        src/external/jana_0.7.7p1/src/JANA/JCalibrationCCDB.h
        src/external/jana_0.7.7p1/src/JANA/JCalibrationFile.cc
        src/external/jana_0.7.7p1/src/JANA/JCalibrationFile.h
        src/external/jana_0.7.7p1/src/JANA/JCalibrationGenerator.h
        src/external/jana_0.7.7p1/src/JANA/JCalibrationGeneratorCCDB.h
        src/external/jana_0.7.7p1/src/JANA/jerror.h
        src/external/jana_0.7.7p1/src/JANA/JEvent.cc
        src/external/jana_0.7.7p1/src/JANA/JEvent.h
        src/external/jana_0.7.7p1/src/JANA/JEventLoop.cc
        src/external/jana_0.7.7p1/src/JANA/JEventLoop.h
        src/external/jana_0.7.7p1/src/JANA/JEventProcessor.cc
        src/external/jana_0.7.7p1/src/JANA/JEventProcessor.h
        src/external/jana_0.7.7p1/src/JANA/JEventSink.cc
        src/external/jana_0.7.7p1/src/JANA/JEventSink.h
        src/external/jana_0.7.7p1/src/JANA/JEventSource.cc
        src/external/jana_0.7.7p1/src/JANA/JEventSource.h
        src/external/jana_0.7.7p1/src/JANA/JEventSource_NULL.h
        src/external/jana_0.7.7p1/src/JANA/JEventSourceGenerator.h
        src/external/jana_0.7.7p1/src/JANA/JEventSourceGenerator_NULL.h
        src/external/jana_0.7.7p1/src/JANA/JException.cc
        src/external/jana_0.7.7p1/src/JANA/JException.h
        src/external/jana_0.7.7p1/src/JANA/JFactory.h
        src/external/jana_0.7.7p1/src/JANA/JFactory_base.cc
        src/external/jana_0.7.7p1/src/JANA/JFactory_base.h
        src/external/jana_0.7.7p1/src/JANA/JFactoryGenerator.h
        src/external/jana_0.7.7p1/src/JANA/JGeometry.h
        src/external/jana_0.7.7p1/src/JANA/JGeometryMYSQL.cc
        src/external/jana_0.7.7p1/src/JANA/JGeometryMYSQL.h
        src/external/jana_0.7.7p1/src/JANA/JGeometryXML.cc
        src/external/jana_0.7.7p1/src/JANA/JGeometryXML.h
        src/external/jana_0.7.7p1/src/JANA/JObject.cc
        src/external/jana_0.7.7p1/src/JANA/JObject.h
        src/external/jana_0.7.7p1/src/JANA/JObject_LinkDef.h
        src/external/jana_0.7.7p1/src/JANA/JParameter.cc
        src/external/jana_0.7.7p1/src/JANA/JParameter.h
        src/external/jana_0.7.7p1/src/JANA/JParameterManager.cc
        src/external/jana_0.7.7p1/src/JANA/JParameterManager.h
        src/external/jana_0.7.7p1/src/JANA/JResourceManager.cc
        src/external/jana_0.7.7p1/src/JANA/JResourceManager.h
        src/external/jana_0.7.7p1/src/JANA/JStreamLog.cc
        src/external/jana_0.7.7p1/src/JANA/JStreamLog.h
        src/external/jana_0.7.7p1/src/JANA/JStreamLogBuffer.cc
        src/external/jana_0.7.7p1/src/JANA/JStreamLogBuffer.h
        src/external/jana_0.7.7p1/src/JANA/JThread.h
        src/external/jana_0.7.7p1/src/JANA/JVersion.h
        src/external/jana_0.7.7p1/src/JANA/make_xcode
        src/external/jana_0.7.7p1/src/JANA/md5.c
        src/external/jana_0.7.7p1/src/JANA/md5.h
        )
target_include_directories(jana1 PUBLIC src/external/jana_0.7.7p1/src/)

target_link_libraries(jana1 ${CMAKE_DL_LIBS})
target_link_libraries(jana1 Threads::Threads)


# Everything except JANA1 gets JANA2 includes
add_subdirectory(/home/nbrei/src/jana2 /home/nbrei/src/jana2/cmake-build-debug)
include_directories(/home/nbrei/src/jana2/src/lib)


#find_package(ROOT 6.0.0 REQUIRED COMPONENTS RIO Tree Hist)
find_package(ROOT 6 REQUIRED)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})


#ROOT_GENERATE_DICTIONARY(G__epicsData src/libraries/EPICS/epicsData.h LINKDEF src/libraries/EPICS/epicsData_LinkDef.h)
#ROOT_GENERATE_DICTIONARY(G__ src/libraries/EPICS/epicsData.h LINKDEF src/libraries/EPICS/epicsData_LinkDef.h)

# Need to generate ROOT dictionaries for these headers, which do not have a linkdef. These all go into one build artifact.
root_generate_dictionary(G__bdx
        src/libraries/DAQ/fa250Hit.h
        src/libraries/DAQ/fa250Mode1Hit.h
        src/libraries/DAQ/fa250ConvertedHit.h
        src/libraries/DAQ/fa250Mode1CalibPedSubHitFFT.h
        src/libraries/DAQ/fa250Mode1CalibPedSubHit.h
        src/libraries/Calorimeter/CalorimeterCluster.h
        src/libraries/IntVeto/IntVetoHit.h
        src/libraries/ExtVeto/ExtVetoHit.h
        src/libraries/EPICS/epicsData.h
        src/libraries/EventBuilder/CataniaEvent.h
        src/libraries/BDXmini/triggerDataBDXmini.h
        )

# Need to generate ROOT dictionaries for these headers, which each have their own linkdef
# Each of these dictionaries becomes a separate build artifact.
# TODO: This seems 'wrong'. ROOT CMake assumes we provide a single project-wide linkdef, but in reality we provide
#       linkdefs specific to each JObject.

root_generate_dictionary(G__bdx_calorimeterhit src/libraries/Calorimeter/CalorimeterHit.h LINKDEF src/libraries/Calorimeter/CalorimeterHit_LinkDef.h)
root_generate_dictionary(G__bdx_calorimeterdigihit src/libraries/Calorimeter/CalorimeterDigiHit.h LINKDEF src/libraries/Calorimeter/CalorimeterDigiHit_LinkDef.h)
root_generate_dictionary(G__bdx_calorimetermcrealhit src/libraries/Calorimeter/CalorimeterMCRealHit.h LINKDEF src/libraries/Calorimeter/CalorimeterMCRealHit_LinkDef.h)
root_generate_dictionary(G__bdx_calorimetersipmhit src/libraries/Calorimeter/CalorimeterSiPMHit.h LINKDEF src/libraries/Calorimeter/CalorimeterSiPMHit_LinkDef.h)
root_generate_dictionary(G__bdx_teventheader src/libraries/EventBuilder/TEventHeader.h LINKDEF src/libraries/EventBuilder/TEventHeader_LinkDef.h)
root_generate_dictionary(G__bdx_tevent src/libraries/EventBuilder/TEvent.h LINKDEF src/libraries/EventBuilder/TEvent_LinkDef.h)
root_generate_dictionary(G__bdx_extvetodigihit src/libraries/ExtVeto/ExtVetoDigiHit.h LINKDEF src/libraries/ExtVeto/ExtVetoDigiHit_LinkDef.h)
root_generate_dictionary(G__bdx_intvetodigihit src/libraries/IntVeto/IntVetoDigiHit.h LINKDEF src/libraries/IntVeto/IntVetoDigiHit_LinkDef.h)
root_generate_dictionary(G__bdx_translationtable src/libraries/TT/TranslationTable.h LINKDEF src/libraries/TT/TranslationTable_LinkDef.h)
root_generate_dictionary(G__bdx_bdxobject src/libraries/system/BDXObject.h LINKDEF src/libraries/system/BDXObject_LinkDef.h)


add_library(bdxreco STATIC
        src/libraries/BDXmini
        src/libraries/BDXmini/JFactoryGenerator_BDXmini.h
        src/libraries/BDXmini/triggerDataBDXmini.cc
        src/libraries/BDXmini/triggerDataBDXmini.h
        src/libraries/BDXmini/triggerDataBDXmini_factory.cc
        src/libraries/BDXmini/triggerDataBDXmini_factory.h

        src/libraries/Calorimeter/CalorimeterCluster.cc
        src/libraries/Calorimeter/CalorimeterCluster.h
        src/libraries/Calorimeter/CalorimeterCluster_factory.cc
        src/libraries/Calorimeter/CalorimeterCluster_factory.h
        src/libraries/Calorimeter/CalorimeterDigiHit.h
        src/libraries/Calorimeter/CalorimeterDigiHit_factory.cc
        src/libraries/Calorimeter/CalorimeterDigiHit_factory.h
        src/libraries/Calorimeter/CalorimeterDigiHit_factory_MC.cc
        src/libraries/Calorimeter/CalorimeterDigiHit_factory_MC.h
        src/libraries/Calorimeter/CalorimeterDigiHit_LinkDef.h
        src/libraries/Calorimeter/Calorimeterfa250Converter.cc
        src/libraries/Calorimeter/Calorimeterfa250Converter.h
        src/libraries/Calorimeter/Calorimeterfa250Converter_factory.cc
        src/libraries/Calorimeter/Calorimeterfa250Converter_factory.h
        src/libraries/Calorimeter/CalorimeterHit.cc
        src/libraries/Calorimeter/CalorimeterHit.h
        src/libraries/Calorimeter/CalorimeterHit_factory.cc
        src/libraries/Calorimeter/CalorimeterHit_factory.h
        src/libraries/Calorimeter/CalorimeterHit_LinkDef.h
        src/libraries/Calorimeter/CalorimeterMCRealHit.h
        src/libraries/Calorimeter/CalorimeterMCRealHit_factory.cc
        src/libraries/Calorimeter/CalorimeterMCRealHit_factory.h
        src/libraries/Calorimeter/CalorimeterMCRealHit_LinkDef.h
        src/libraries/Calorimeter/CalorimeterSiPMHit.h
        src/libraries/Calorimeter/CalorimeterSiPMHit_factory.cc
        src/libraries/Calorimeter/CalorimeterSiPMHit_factory.h
        src/libraries/Calorimeter/CalorimeterSiPMHit_LinkDef.h
        src/libraries/Calorimeter/JFactoryGenerator_Calorimeter.h

        src/libraries/DAQ/DAQCalibrationHandler.cc
        src/libraries/DAQ/DAQCalibrationHandler.h
        src/libraries/DAQ/epicsRawData.h
        src/libraries/DAQ/eventData.h
        src/libraries/DAQ/fa250ConvertedHit.h
        src/libraries/DAQ/fa250Converter.h
        src/libraries/DAQ/fa250Hit.h
        src/libraries/DAQ/fa250Mode1CalibPedSubHit.cc
        src/libraries/DAQ/fa250Mode1CalibPedSubHit_factory.cc
        src/libraries/DAQ/fa250Mode1CalibPedSubHit_factory.h
        src/libraries/DAQ/fa250Mode1CalibPedSubHitFFT.cc
        src/libraries/DAQ/fa250Mode1CalibPedSubHitFFT.h
        src/libraries/DAQ/fa250Mode1CalibPedSubHitFFT_factory.cc
        src/libraries/DAQ/fa250Mode1CalibPedSubHitFFT_factory.h
        src/libraries/DAQ/fa250Mode1Hit.cc
        src/libraries/DAQ/fa250Mode7Hit.h
        src/libraries/DAQ/fa250WaveboardV1Hit.cc
        src/libraries/DAQ/fa250WaveboardV1Hit.h
        src/libraries/DAQ/JEventSourceEVIODAQ.cc
        src/libraries/DAQ/JEventSourceEVIODAQ.h
        src/libraries/DAQ/JEventSourceTRIDASDAQ.cc
        src/libraries/DAQ/JEventSourceTRIDASDAQ.h
        src/libraries/DAQ/JFactoryGenerator_DAQ.h

        src/libraries/EPICS/epicsData.cc
        src/libraries/EPICS/epicsData.h
        src/libraries/EPICS/epicsData_factory.cc
        src/libraries/EPICS/epicsData_factory.h
        src/libraries/EPICS/epicsData_LinkDef.h
        src/libraries/EPICS/JFactoryGenerator_EPICS.h

        src/libraries/Paddles/JFactoryGenerator_Paddles.h
        src/libraries/Paddles/PaddlesDigiHit.h
        src/libraries/Paddles/PaddlesDigiHit_factory.cc
        src/libraries/Paddles/PaddlesDigiHit_factory.h
        src/libraries/Paddles/PaddlesDigiHit_factory_MC.cc
        src/libraries/Paddles/PaddlesDigiHit_factory_MC.h
        src/libraries/Paddles/Paddlesfa250Converter.cc
        src/libraries/Paddles/Paddlesfa250Converter.h
        src/libraries/Paddles/Paddlesfa250Converter_factory.cc
        src/libraries/Paddles/Paddlesfa250Converter_factory.h
        src/libraries/Paddles/PaddlesHit.h
        src/libraries/Paddles/PaddlesHit_factory.cc
        src/libraries/Paddles/PaddlesHit_factory.h
        src/libraries/Paddles/PaddlesPMTHit.h
        src/libraries/Paddles/PaddlesPMTHit_factory.cc
        src/libraries/Paddles/PaddlesPMTHit_factory.h

        src/libraries/EventBuilder/CataniaEvent.h
        src/libraries/EventBuilder/CataniaEvent_factory.cc
        src/libraries/EventBuilder/CataniaEvent_factory.h
        src/libraries/EventBuilder/JFactoryGenerator_EventBuilder.h
        src/libraries/EventBuilder/MCEvent.h
        src/libraries/EventBuilder/TEvent.cc
        src/libraries/EventBuilder/TEvent.h
        src/libraries/EventBuilder/TEvent_factory.cc
        src/libraries/EventBuilder/TEvent_factory.h
        src/libraries/EventBuilder/TEvent_factory_BDXmini.cc
        src/libraries/EventBuilder/TEvent_factory_BDXmini.h
        src/libraries/EventBuilder/TEvent_factory_CataniaProto2.cc
        src/libraries/EventBuilder/TEvent_factory_CataniaProto2.h
        src/libraries/EventBuilder/TEvent_factory_FullMC.cc
        src/libraries/EventBuilder/TEvent_factory_FullMC.h
        src/libraries/EventBuilder/TEvent_factory_JLabFlux.cc
        src/libraries/EventBuilder/TEvent_factory_JLabFlux.h
        src/libraries/EventBuilder/TEvent_LinkDef.h
        src/libraries/EventBuilder/TEventHeader.cc
        src/libraries/EventBuilder/TEventHeader.h
        src/libraries/EventBuilder/TEventHeader_LinkDef.h

        src/libraries/ExtVeto/ExtVetoDigiHit.h
        src/libraries/ExtVeto/ExtVetoDigiHit_factory.cc
        src/libraries/ExtVeto/ExtVetoDigiHit_factory.h
        src/libraries/ExtVeto/ExtVetoDigiHit_factory_MC.cc
        src/libraries/ExtVeto/ExtVetoDigiHit_factory_MC.h
        src/libraries/ExtVeto/ExtVetoDigiHit_LinkDef.h
        src/libraries/ExtVeto/ExtVetofa250Converter.cc
        src/libraries/ExtVeto/ExtVetofa250Converter.h
        src/libraries/ExtVeto/ExtVetofa250Converter_factory.cc
        src/libraries/ExtVeto/ExtVetofa250Converter_factory.h
        src/libraries/ExtVeto/ExtVetoHit.h
        src/libraries/ExtVeto/ExtVetoHit_factory.cc
        src/libraries/ExtVeto/ExtVetoHit_factory.h
        src/libraries/ExtVeto/ExtVetoPMTHit.h
        src/libraries/ExtVeto/ExtVetoPMTHit_factory.cc
        src/libraries/ExtVeto/ExtVetoPMTHit_factory.h
        src/libraries/ExtVeto/ExtVetoSummary.h
        src/libraries/ExtVeto/ExtVetoSummary_factory.cc
        src/libraries/ExtVeto/ExtVetoSummary_factory.h
        src/libraries/ExtVeto/JFactoryGenerator_ExtVeto.h

        src/libraries/IntVeto/IntVetoDigiHit.cc
        src/libraries/IntVeto/IntVetoDigiHit.h
        src/libraries/IntVeto/IntVetoDigiHit_factory.cc
        src/libraries/IntVeto/IntVetoDigiHit_factory.h
        src/libraries/IntVeto/IntVetoDigiHit_factory_MC.cc
        src/libraries/IntVeto/IntVetoDigiHit_factory_MC.h
        src/libraries/IntVeto/IntVetoDigiHit_LinkDef.h
        src/libraries/IntVeto/IntVetofa250Converter.cc
        src/libraries/IntVeto/IntVetofa250Converter.h
        src/libraries/IntVeto/IntVetofa250Converter_factory.cc
        src/libraries/IntVeto/IntVetofa250Converter_factory.h
        src/libraries/IntVeto/IntVetoHit.cc
        src/libraries/IntVeto/IntVetoHit.h
        src/libraries/IntVeto/IntVetoHit_factory.cc
        src/libraries/IntVeto/IntVetoHit_factory.h
        src/libraries/IntVeto/IntVetoHit_LinkDef.h
        src/libraries/IntVeto/IntVetoSiPMHit.h
        src/libraries/IntVeto/IntVetoSiPMHit_factory.cc
        src/libraries/IntVeto/IntVetoSiPMHit_factory.h
        src/libraries/IntVeto/IntVetoSummary.cc
        src/libraries/IntVeto/IntVetoSummary.h
        src/libraries/IntVeto/IntVetoSummary_factory.cc
        src/libraries/IntVeto/IntVetoSummary_factory.h
        src/libraries/IntVeto/JFactoryGenerator_IntVeto.h

        src/libraries/TT/JFactoryGenerator_TT.h
        src/libraries/TT/TranslationTable.cc
        src/libraries/TT/TranslationTable.h
        src/libraries/TT/TranslationTable_factory.cc
        src/libraries/TT/TranslationTable_factory.h
        src/libraries/TT/TranslationTable_LinkDef.h

        src/libraries/system/BDXEventProcessor.cc
        src/libraries/system/BDXEventProcessor.h
        src/libraries/system/BDXObject.cc
        src/libraries/system/BDXObject.h
        src/libraries/system/BDXObject_LinkDef.h
        src/libraries/system/CalibrationHandler.h
        src/libraries/system/JEventSourceEVIOGenerator.cc
        src/libraries/system/JEventSourceEVIOGenerator.h
        src/libraries/system/JEventSourceTRIDASGenerator.cc
        src/libraries/system/JEventSourceTRIDASGenerator.h
        src/libraries/system/JFactoryGenerator_system.h
        src/libraries/system/JOutput.h
        src/libraries/system/JROOTOutput.cc
        src/libraries/system/JROOTOutput.h
        src/libraries/system/string_utilities.h
        src/libraries/system/BDXCalibrationService.h

        # Generated ROOT dictionaries
        G__bdx.cxx
        G__bdx_calorimeterhit.cxx
        G__bdx_calorimeterdigihit.cxx
        G__bdx_calorimetermcrealhit.cxx
        G__bdx_calorimetersipmhit.cxx
        G__bdx_teventheader.cxx
        G__bdx_tevent.cxx
        G__bdx_extvetodigihit.cxx
        G__bdx_intvetodigihit.cxx
        G__bdx_translationtable.cxx
        G__bdx_bdxobject.cxx
        )


target_link_libraries(bdxreco jana2)
target_link_libraries(bdxreco evioReader)
target_link_libraries(bdxreco tridas)
target_link_libraries(bdxreco ${ROOT_LIBRARIES})


add_executable(bdxreco_cli
        src/programs/bdxReco/bdxReco.cc
        #src/programs/bdxReco/main.cc
        )

target_link_libraries(bdxreco_cli bdxreco)
target_link_libraries(bdxreco Threads::Threads)


