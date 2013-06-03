/**
 * ----------------------------------------------------------------------
 *  Copyright (C) 2010 Numenta Inc. All rights reserved.
 *
 *  The information and source code contained herein is the
 *  exclusive property of Numenta Inc. No part of this software
 *  may be used, reproduced, stored or distributed in any form,
 *  without explicit written authorization from Numenta Inc.
 * ----------------------------------------------------------------------
*/

/** @file
 * YAMLUtils unit tests
 */

#ifndef NTA_YAMLUTILS_TEST_HPP
#define NTA_YAMLUTILS_TEST_HPP

//----------------------------------------------------------------------

#include <nta/test/Tester.hpp>

//----------------------------------------------------------------------

namespace nta {

  struct YAMLUtilsTest : public Tester
  {
    virtual ~YAMLUtilsTest() {}
    virtual void RunTests();
  };
}

#endif // NTA_YAMLUTILS_TEST_HPP