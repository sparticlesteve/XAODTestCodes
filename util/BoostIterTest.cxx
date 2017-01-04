// System includes
#include <iostream>

// ROOT includes
#include "TFile.h"

// Boost includes
#include "boost/iterator/filter_iterator.hpp"

// xAOD infrastructure includes
#ifdef ROOTCORE
#  include "xAODRootAccess/Init.h"
#  include "xAODRootAccess/TEvent.h"
#endif // ROOTCORE

// xAOD EDM includes
#include "AthContainers/AuxElement.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"

//-----------------------------------------------------------------------------
// Error checking macro
//-----------------------------------------------------------------------------
#define CHECK( ARG )                                 \
  do {                                               \
    const bool result = ARG;                         \
    if(!result) {                                    \
      std::cerr << "Failed to execute: \"%s\""       \
                << #ARG << std::endl;                \
      return 1;                                      \
    }                                                \
  } while( false )

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

// Input file name
const char* fileName = "/afs/cern.ch/user/a/asgbase/patspace/xAODs/r7725/mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.AOD.e3698_s2608_s2183_r7725_r7676/AOD.07915862._000100.pool.root.1";

// Event to use
const int testEntry = 0;

//-----------------------------------------------------------------------------
// Accessor predicate
//-----------------------------------------------------------------------------
template <class ContainerType>
class AccessorPredicateRange
{
  public:
    using ElementType_t = typename ContainerType::const_value_type;
    using PredType_t = std::function<bool(ElementType_t)>;
    using BaseIterType_t = xAOD::ElectronContainer::const_iterator;
    using IterType_t = boost::filter_iterator<PredType_t, BaseIterType_t>;

    /// Constructor
    AccessorPredicateRange(const ContainerType* container,
                           const SG::AuxElement::ConstAccessor<char>& acc)
      : m_container(container)
    {
      m_pred = [acc](ElementType_t p) {
        return  acc(*p);
      };
    }

    /// Range methods
    IterType_t begin() {
      return boost::make_filter_iterator(m_pred, m_container->begin(), m_container->end());
    }
    IterType_t end() {
      return boost::make_filter_iterator(m_pred, m_container->end(), m_container->end());
    }

  private:
    const ContainerType* m_container;
    PredType_t m_pred;
};

//-----------------------------------------------------------------------------
// Convenience function to auto-deduce template type
//-----------------------------------------------------------------------------
template <class ContainerType>
AccessorPredicateRange<ContainerType>
MakePredicateRange(const ContainerType* container,
                   const SG::AuxElement::ConstAccessor<char>& acc)
{
  return AccessorPredicateRange<ContainerType>(container, acc);
}


//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main()
{
  // Setup input and services
  CHECK( xAOD::Init() );
  TFile inputFile(fileName);
  xAOD::TEvent event;
  CHECK( event.readFrom(&inputFile) );
  event.getEntry(testEntry);

  // Get electrons
  const xAOD::ElectronContainer* electrons = nullptr;
  CHECK( event.retrieve(electrons, "Electrons") );

  // The ID accessor
  SG::AuxElement::ConstAccessor<char> idAcc("Loose");

  // Print out some ID flags
  std::cout << "All electrons:" << std::endl;
  for(const auto electron : *electrons) {
    bool pass = idAcc(*electron);
    std::cout << "  pt " << electron->pt()*0.001
              << " id "  << pass
              << std::endl;
  }

  // Print out the selected objects only
  std::cout << "Selected electrons:" << std::endl;
  for(auto electron : MakePredicateRange(electrons, idAcc)) {
    bool pass = idAcc(*electron);
    std::cout << "  pt " << electron->pt()*0.001
              << " id "  << pass
              << std::endl;
  }

  return 0;
}
