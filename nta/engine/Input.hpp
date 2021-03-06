/*
 * ---------------------------------------------------------------------
 * Numenta Platform for Intelligent Computing (NuPIC)
 * Copyright (C) 2013, Numenta, Inc.  Unless you have purchased from
 * Numenta, Inc. a separate commercial license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * http://numenta.org/licenses/
 * ---------------------------------------------------------------------
 */

/** @file 
 * Definition of the Input class
 * This class is internal, and is not wrapped.
 *
*/

#ifndef NTA_INPUT_HPP
#define NTA_INPUT_HPP

#ifdef SWIG
#error "Input class should not be wrapped"
#endif


#include <vector>
#include <nta/types/types.hpp>
#include <nta/ntypes/Array.hpp>

namespace nta
{
  class Link;
  class Region;
  class Output;

  /**
   * Input represents a named input to a region (e.g. bottomUpIn)
   * 
   * Input is not available in the public API, but is visible by 
   * the RegionImpl
   * 
   * TBD: identify methods that may be called by RegionImpl -- this
   * is the internal "public interface"
   */

  class Input
  {
  public:
    Input(Region& region, NTA_BasicType type, bool isRegionLevel);
    ~Input();

    /*
     * Inputs need to know their own name for error messages
     */
    void setName(const std::string& name);

    const std::string& getName() const;

    /*
     * Create a new link and add it to this input
     * Also adds the link to the list of links on the output
     */
    void
    addLink(const std::string& linkType, const std::string& linkParams, 
            Output* srcOutput);
    
    /*
     * Locate an existing link. Returns NULL if no link exists
     * Called by Network::unlink and internally when adding a link
     */
    Link* 
    findLink(const std::string& srcRegionName, 
             const std::string& srcOutputName);

    /*
     * Removing an existing link
     * Called in four cases
     * 1. Network::unlink()
     * 2. Network::removeRegion(srcRegion)
     * 3. Network::removeRegion(destRegion)
     * 4. Network::~Network()
     * 
     * It is an error to call this if our containing region 
     * is unitialized. 
     * 
     * Sets the link pointer to NULL on return (to avoid a dangling reference)
     */
    void
    removeLink(Link*& link);

    // Make input data available. Called by Region::prepareInputs()
    void
    prepare();
    
    const Array &
    getData() const;
    
    Region&
    getRegion();

    const std::vector<Link*>&
    getLinks();

    bool
    isRegionLevel();

    /*
     * Called by Region::evaluateLinks() as part
     * of network initialization.
     * 
     * 1. Tries to make sure that dimensions at both ends
     *    of a link are specified by calling setSourceDimensions()
     *    if possible, and then calling getDestDimensions()
     * 2. Ensures that region dimensions are consistent with 
     *    either by setting destination region dimensions (this is
     *    where links "induce" dimensions) or by raising an exception
     *    if they are inconsistent.
     *
     */
    size_t
    evaluateLinks();

    // After the input has all the information it needs, 
    // it is initialized by this method. Sets up volatile data 
    // structures (e.g. the input buffer) are set up
    // 
    void
    initialize();

    bool
    isInitialized();

    /* ------------ Methods normally called by the RegionImpl ------------- */

    // Get splitter map from an initialized input
    // See documentation for Link::getSplitterMap for
    // description of splitter map. 
    typedef std::vector< std::vector<size_t> > SplitterMap;

    const SplitterMap& getSplitterMap() const;

    // explicitly instantiated for various types
    template <typename T> void getInputForNode(size_t nodeIndex, std::vector<T>& input) const;

  private:
    Region& region_;
    // buffer is concatenation of input buffers (after prepare), or, 
    // if zeroCopyEnabled it points to the connected output
    bool isRegionLevel_;


    // Use a vector of links because order is
    // important.
    std::vector<Link*> links_;

    bool zeroCopyEnabled_;

    // volatile (non-serialized) state
    bool initialized_;
    Array data_;

    /* 
     * cached splitter map -- only created if requested
     * mutable because getSplitterMap() is const and logically
     * getting the splitter map doesn't change the Input
     */
    mutable SplitterMap splitterMap_;


    /*
     * Cache of information about link offsets so we can 
     * easily copy data from each link.
     * the first link starts at offset 0
     * the next link starts at offset 0 + size(link[0])
     */
    std::vector<size_t> linkOffsets_;

    size_t size_; // computed from links

    // Useful for us to know our own name
    std::string name_;

    // Internal methods
    
    /*
     * uninitialize is called by removeLink
     * and in our destructor. It is an error
     * to call it if our region is initialized.
     * It frees the input buffer and the splitter map
     * but does not affect the links.
     */
    void
    uninitialize();



  };

}

#endif // NTA_INPUT_HPP
