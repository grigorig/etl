///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2019 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#include "UnitTest++.h"

#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include "etl/indirect_vector.h"
#include "etl/vector.h"
#include "etl/pool.h"

#include "data.h"

namespace
{
  SUITE(test_indirect_vector_external_buffer)
  {
    static const size_t SIZE = 12;

    typedef TestDataNDC<std::string> NDC;
    typedef TestDataDC<std::string>  DC;

    using LookupDC = etl::vector<DC*, SIZE>;
    using LookupDCLarger = etl::vector<DC*, SIZE + 1>;
    using PoolDC   = etl::pool<DC, SIZE>;

    using LookupNDC = etl::vector<NDC*, SIZE>;
    using PoolNDC   = etl::pool<NDC, SIZE>;

    using LookupInt = etl::vector<int*, SIZE>;
    using PoolInt   = etl::pool<int, SIZE>;

    using LookupUniquePtr = etl::vector<std::unique_ptr<uint32_t>*, SIZE>;
    using PoolUniquePtr   = etl::pool<std::unique_ptr<uint32_t>, SIZE>;

    static NDC ndc("NDC");

    typedef etl::indirect_vector<NDC, 0U> DataNDC;
    typedef etl::iindirect_vector<NDC>    IDataNDC;
    typedef std::vector<NDC>              CompareDataNDC;

    typedef etl::indirect_vector<DC, 0U> DataDC;
    typedef etl::iindirect_vector<DC>    IDataDC;
    typedef std::vector<DC>              CompareDataDC;

    CompareDataNDC initial_data;
    CompareDataNDC less_data;
    CompareDataNDC greater_data;
    CompareDataNDC shorter_data;
    CompareDataNDC different_data;
    CompareDataNDC insert_data;
    CompareDataNDC ordered_data;
    CompareDataNDC part_ordered_data;
    CompareDataNDC part_reverse_ordered_data;
    CompareDataNDC stable_default_ordered_data;
    CompareDataNDC stable_reverse_ordered_data;
    CompareDataNDC stable_greater_ordered_data;
    CompareDataNDC stable_greater_reverse_ordered_data;
    CompareDataNDC unordered_data;
    CompareDataNDC stable_part_ordered_data;
    CompareDataNDC stable_part_greater_ordered_data;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        NDC n[]                             = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4"), NDC("4"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_insert[]                      = { NDC("11"), NDC("12"), NDC("13") };
        NDC n_less[]                        = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4"), NDC("4"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_greater[]                     = { NDC("0"), NDC("1"), NDC("2"), NDC("4"), NDC("4"), NDC("4"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };

        NDC n_unordered[]                   = { NDC("1"), NDC("4", 1), NDC("7"), NDC("4", 2), NDC("2"), NDC("4", 3), NDC("3"), NDC("6"), NDC("9"), NDC("0"), NDC("8"), NDC("5") };

        NDC n_ordered[]                     = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4"), NDC("4"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_stable_default_ordered[]      = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4", 1), NDC("4", 2), NDC("4", 3), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_stable_reverse_ordered[]      = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4", 3), NDC("4", 2), NDC("4", 1), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_stable_greater_ordered[]      = { NDC("9"), NDC("8"), NDC("7"), NDC("6"), NDC("5"), NDC("4", 1), NDC("4", 2), NDC("4", 3), NDC("3"), NDC("2"), NDC("1"), NDC("0") };
        NDC n_stable_greater_reverse_ordered_data[] = { NDC("9"), NDC("8"), NDC("7"), NDC("6"), NDC("5"), NDC("4", 3), NDC("4", 2), NDC("4", 1), NDC("3"), NDC("2"), NDC("1"), NDC("0") };

        NDC n_part_ordered[]                = { NDC("1"), NDC("4", 1), NDC("0"), NDC("2"), NDC("3"), NDC("4"), NDC("4"), NDC("6"), NDC("7"), NDC("9"), NDC("8"), NDC("5") };
        NDC n_part_reverse_ordered[]        = { NDC("1"), NDC("4"), NDC("9"), NDC("7"), NDC("6"), NDC("4"), NDC("4"), NDC("3"), NDC("2"), NDC("0"), NDC("8"), NDC("5") };

        NDC n_stable_part_ordered[]         = { NDC("1"), NDC("4", 1), NDC("0"), NDC("2"), NDC("3"), NDC("4", 2), NDC("4", 3), NDC("6"), NDC("7"), NDC("9"), NDC("8"), NDC("5") };
        NDC n_stable_part_greater_ordered[] = { NDC("1"), NDC("4", 1), NDC("9"), NDC("7"), NDC("6"), NDC("4", 2), NDC("4", 3), NDC("3"), NDC("2"), NDC("0"), NDC("8"), NDC("5") };


        initial_data.assign(std::begin(n), std::end(n));
        insert_data.assign(std::begin(n_insert), std::end(n_insert));
        less_data.assign(std::begin(n_less), std::end(n_less));
        greater_data.assign(std::begin(n_greater), std::end(n_greater));
        shorter_data.assign(std::begin(n_greater), std::end(n_greater) - 1);
        different_data.assign(initial_data.rbegin(), initial_data.rend());
        ordered_data.assign(std::begin(n_ordered), std::end(n_ordered));
        part_ordered_data.assign(std::begin(n_part_ordered), std::end(n_part_ordered));
        part_reverse_ordered_data.assign(std::begin(n_part_reverse_ordered), std::end(n_part_reverse_ordered));
        unordered_data.assign(std::begin(n_unordered), std::end(n_unordered));
        stable_default_ordered_data.assign(std::begin(n_stable_default_ordered), std::end(n_stable_default_ordered));
        stable_reverse_ordered_data.assign(std::begin(n_stable_reverse_ordered), std::end(n_stable_reverse_ordered));
        stable_greater_ordered_data.assign(std::begin(n_stable_greater_ordered), std::end(n_stable_greater_ordered));
        stable_greater_reverse_ordered_data.assign(std::begin(n_stable_greater_reverse_ordered_data), std::end(n_stable_greater_reverse_ordered_data));
        stable_part_ordered_data.assign(std::begin(n_stable_part_ordered), std::end(n_stable_part_ordered));
        stable_part_greater_ordered_data.assign(std::begin(n_stable_part_greater_ordered), std::end(n_stable_part_greater_ordered));
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      LookupDC lookup;
      PoolDC   pool;

      DataDC data(lookup, pool);

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_buffer_mismatch)
    {
      LookupDCLarger lookup;
      PoolDC         pool;

      CHECK_THROW(DataDC data(lookup, pool), etl::indirect_vector_buffer_missmatch);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_ivector)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      int current_count = NDC::get_instance_count();

      DataNDC* p = new DataNDC(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);
      delete p;

      DataNDC* pdata = new DataNDC(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);
      CHECK_EQUAL(int(current_count + INITIAL_SIZE), NDC::get_instance_count());

      IDataNDC* pidata = pdata;
      delete pidata;
      CHECK_EQUAL(current_count, NDC::get_instance_count());
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
    {
      LookupDC lookup;
      PoolDC   pool;

      DataDC data(lookup, pool);

      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      LookupDC lookup;
      PoolDC   pool;

      const size_t INITIAL_SIZE = 5;
      DataDC data(INITIAL_SIZE, lookup, pool);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      std::vector<NDC> compare_data(INITIAL_SIZE, INITIAL_VALUE);
      DataNDC data(size_t(5), NDC("1"), lookup, pool);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      LookupDC lookup;
      PoolDC   pool;

      CHECK_THROW(DataDC data(SIZE + 1, lookup, pool), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST(test_constructor_initializer_list)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data = { NDC("0"), NDC("1"), NDC("2"), NDC("3") };
      DataNDC data({ NDC("0"), NDC("1"), NDC("2"), NDC("3") }, lookup, pool);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      DataNDC data(initial_data.begin(), initial_data.end(), lookup1, pool1);
      DataNDC data2(data, lookup2, pool2);
      CHECK(data2 == data);

      data2[2] = NDC("X");
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      typedef etl::indirect_vector<std::unique_ptr<uint32_t>, 0> Data;

      LookupUniquePtr lookup1;
      PoolUniquePtr   pool1;

      LookupUniquePtr lookup2;
      PoolUniquePtr   pool2;

      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));

      Data data1(lookup1, pool1);
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      Data data2(std::move(data1), lookup2, pool2);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      CHECK_EQUAL(1U, *data2[0]);
      CHECK_EQUAL(2U, *data2[1]);
      CHECK_EQUAL(3U, *data2[2]);
      CHECK_EQUAL(4U, *data2[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      DataNDC data(initial_data.begin(), initial_data.end(), lookup1, pool1);
      DataNDC other_data(lookup2, pool2);

      other_data = data;

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_move_assignment)
    {
      const size_t SIZE = 10U;
      typedef etl::indirect_vector<std::unique_ptr<uint32_t>, 0> Data;

      LookupUniquePtr lookup1;
      PoolUniquePtr   pool1;

      LookupUniquePtr lookup2;
      PoolUniquePtr   pool2;

      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));

      Data data1(lookup1, pool1);
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      Data data2(lookup2, pool2);
      data2 = std::move(data1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      CHECK_EQUAL(1U, *data2[0]);
      CHECK_EQUAL(2U, *data2[1]);
      CHECK_EQUAL(3U, *data2[2]);
      CHECK_EQUAL(4U, *data2[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      DataNDC data1(initial_data.begin(), initial_data.end(), lookup1, pool1);
      DataNDC data2(lookup2, pool2);

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      bool is_equal = std::equal(data1.begin(),
                                 data1.end(),
                                 data2.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_move_assignment_interface)
    {
      LookupUniquePtr lookup1;
      PoolUniquePtr   pool1;

      LookupUniquePtr lookup2;
      PoolUniquePtr   pool2;

      typedef etl::indirect_vector<std::unique_ptr<uint32_t>, 0U> Data;
      typedef etl::iindirect_vector<std::unique_ptr<uint32_t>> IData;

      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));

      Data data1(lookup1, pool1);
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      Data data2(lookup2, pool2);

      IData& idata1 = data1;
      IData& idata2 = data2;

      idata2 = std::move(idata1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      CHECK_EQUAL(1U, *data2[0]);
      CHECK_EQUAL(2U, *data2[1]);
      CHECK_EQUAL(3U, *data2[2]);
      CHECK_EQUAL(4U, *data2[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      DataNDC data(initial_data.begin(), initial_data.end(), lookup1, pool1);
      DataNDC other_data(data, lookup2, pool2);

      other_data = other_data;

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      LookupDC lookup1;
      PoolDC   pool1;

      LookupDC lookup2;
      PoolDC   pool2;

      DataDC data(10, lookup1, pool1);
      const DataDC constData(10, lookup2, pool2);

      CHECK_EQUAL(&data[0], &(*data.begin()));
      CHECK_EQUAL(&constData[0], &(*constData.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      LookupDC lookup1;
      PoolDC   pool1;

      LookupDC lookup2;
      PoolDC   pool2;

      DataDC data(10, lookup1, pool1);
      const DataDC constData(10, lookup2, pool2);

      CHECK_EQUAL(&data[10], &(*data.end()));
      CHECK_EQUAL(&constData[10], &(constData.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      LookupDC lookup;
      PoolDC   pool;

      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      DataDC data(INITIAL_SIZE, lookup, pool);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      std::vector<NDC> compare_data(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      LookupDC lookup;
      PoolDC   pool;

      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      DataDC data(INITIAL_SIZE, lookup, pool);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      LookupDC lookup;
      PoolDC   pool;

      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      DataDC data(INITIAL_SIZE, lookup, pool);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      LookupDC lookup;
      PoolDC   pool;

      DataDC data(lookup, pool);
      data.resize(data.max_size());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(lookup, pool);

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      DataNDC::indirect_pointer p = data.data();

      for (size_t i = 0U; i < data.size(); ++i)
      {
        CHECK(data[i] == **p);
        ++p;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      DataNDC::indirect_const_pointer p = data.data();

      for (size_t i = 0U; i < data.size(); ++i)
      {
        CHECK(data[i] == **p);
        ++p;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(lookup, pool);

      data.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");
      std::vector<NDC> compare_data(INITIAL_SIZE, INITIAL_VALUE);

      DataNDC data(lookup, pool);
      data.assign(INITIAL_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      const NDC INITIAL_VALUE("1");
      std::vector<NDC> compare_data(INITIAL_SIZE, INITIAL_VALUE);

      DataNDC data(lookup, pool);

      CHECK_THROW(data.assign(EXCESS_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data;
      DataNDC data(lookup, pool);

      for (size_t i = 0; i < SIZE; ++i)
      {
        std::string value(" ");
        value[0] = char('A' + i);
        compare_data.push_back(NDC(value));
        data.push_back(NDC(value));
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(lookup, pool);

      for (size_t i = 0; i < SIZE; ++i)
      {
        std::string value(" ");
        value[0] = char('A' + i);
        data.push_back(NDC(value));
      }

      CHECK_THROW(data.push_back(NDC("Z")), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data;
      DataNDC data(lookup, pool);

      for (size_t i = 0; i < SIZE; ++i)
      {
        std::string value(" ");
        value[0] = char('A' + i);
        compare_data.emplace_back(value);
        data.emplace_back(value);
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_unique_ptr)
    {
      LookupUniquePtr lookup;
      PoolUniquePtr   pool;

      etl::indirect_vector<std::unique_ptr<uint32_t>, 0U> data(lookup, pool);

      std::unique_ptr<uint32_t> p1(new uint32_t(1));
      std::unique_ptr<uint32_t> p2(new uint32_t(2));
      std::unique_ptr<uint32_t> p3(new uint32_t(3));
      std::unique_ptr<uint32_t> p4(new uint32_t(4));

      data.push_back(std::move(p1));
      data.push_back(std::move(p2));
      data.push_back(std::move(p3));
      data.push_back(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      CHECK_EQUAL(1, *data[0]);
      CHECK_EQUAL(2, *data[1]);
      CHECK_EQUAL(3, *data[2]);
      CHECK_EQUAL(4, *data[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_int)
    {
      LookupInt lookup;
      PoolInt   pool;

      etl::indirect_vector<int, 0U> data(lookup, pool);

      int p1(1);
      int p2(2);
      int p3(3);
      int p4(4);

      data.insert(data.begin(), p1);
      data.insert(data.begin(), p2);
      data.insert(data.begin(), p3);
      data.insert(data.begin(), p4);

      CHECK_EQUAL(4, data[0]);
      CHECK_EQUAL(3, data[1]);
      CHECK_EQUAL(2, data[2]);
      CHECK_EQUAL(1, data[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_unique_ptr)
    {
      LookupUniquePtr lookup;
      PoolUniquePtr   pool;

      etl::indirect_vector<std::unique_ptr<uint32_t>, 0U> data(lookup, pool);

      std::unique_ptr<uint32_t> p1(new uint32_t(1));
      std::unique_ptr<uint32_t> p2(new uint32_t(2));
      std::unique_ptr<uint32_t> p3(new uint32_t(3));
      std::unique_ptr<uint32_t> p4(new uint32_t(4));

      data.insert(data.begin(), std::move(p1));
      data.insert(data.begin(), std::move(p2));
      data.insert(data.begin(), std::move(p3));
      data.insert(data.begin(), std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      CHECK_EQUAL(1, *data[3]);
      CHECK_EQUAL(4, *data[0]);
      CHECK_EQUAL(3, *data[1]);
      CHECK_EQUAL(2, *data[2]);

    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      compare_data.pop_back();
      compare_data.pop_back();

      data.pop_back();
      data.pop_back();

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back_exception)
    {
      LookupDC lookup;
      PoolDC   pool;

      DataDC data(lookup, pool);

      data.resize(2);

      data.pop_back();
      data.pop_back();

      CHECK_THROW(data.pop_back(), etl::vector_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data(lookup, pool);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.insert(data.begin() + offset, INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const std::string INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data(lookup, pool);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.emplace(data.begin() + offset, INITIAL_VALUE);
        compare_data.emplace(compare_data.begin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE     = SIZE;
      const NDC INITIAL_VALUE("1");
      const NDC UNINITIALISED_VALUE("Z");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE     = 5;
      const size_t INSERT_SIZE      = 3;
      const NDC INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data(lookup, pool);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE  = 4;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data(lookup, pool);

        data.resize(SIZE, NDC("Z"));

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, insert_data.begin(), insert_data.end());
        compare_data.insert(compare_data.begin() + offset, insert_data.begin(), insert_data.end());

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE, lookup, pool);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      compare_data.erase(compare_data.begin() + 2);

      data.erase(data.begin() + 2);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end(), lookup, pool);

      compare_data.erase(compare_data.begin() + 2, compare_data.begin() + 4);

      data.erase(data.begin() + 2, data.begin() + 4);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);
      data.clear();

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      bool is_equal = std::equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      bool is_equal = std::equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end(), lookup, pool);

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      LookupNDC lookup3;
      PoolNDC   pool3;

      LookupNDC lookup4;
      PoolNDC   pool4;

      const DataNDC initial1(initial_data.begin(), initial_data.end(), lookup1, pool1);
      const DataNDC initial2(initial_data.begin(), initial_data.end(), lookup2, pool2);

      CHECK(initial1 == initial2);

      const DataNDC different(different_data.begin(), different_data.end(), lookup3, pool3);

      CHECK(!(initial1 == different));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end(), lookup4, pool4);

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      LookupNDC lookup3;
      PoolNDC   pool3;

      LookupNDC lookup4;
      PoolNDC   pool4;

      const DataNDC initial1(initial_data.begin(), initial_data.end(), lookup1, pool1);
      const DataNDC initial2(initial_data.begin(), initial_data.end(), lookup2, pool2);

      CHECK(!(initial1 != initial2));

      const DataNDC different(different_data.begin(), different_data.end(), lookup3, pool3);

      CHECK(initial1 != different);

      const DataNDC shorter(shorter_data.begin(), shorter_data.end(), lookup4, pool4);

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      LookupNDC lookup3;
      PoolNDC   pool3;

      LookupNDC lookup4;
      PoolNDC   pool4;

      const DataNDC less(less_data.begin(), less_data.end(), lookup1, pool1);
      const DataNDC initial(initial_data.begin(), initial_data.end(), lookup2, pool2);

      CHECK((less < initial) == (less_data < initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end(), lookup3, pool3);

      CHECK((greater < initial) == (greater_data < initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end(), lookup4, pool4);

      CHECK((shorter < initial) == (shorter_data < initial_data));
      CHECK((initial < shorter) == (initial_data < shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than_or_equal)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      LookupNDC lookup3;
      PoolNDC   pool3;

      LookupNDC lookup4;
      PoolNDC   pool4;

      LookupNDC lookup5;
      PoolNDC   pool5;

      const DataNDC less(less_data.begin(), less_data.end(), lookup1, pool1);
      const DataNDC initial(initial_data.begin(), initial_data.end(), lookup2, pool2);

      CHECK((less <= initial) == (less_data <= initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end(), lookup3, pool3);

      CHECK((greater <= initial) == (greater_data <= initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end(), lookup4, pool4);

      CHECK((shorter <= initial) == (shorter_data <= initial_data));
      CHECK((initial <= shorter) == (initial_data <= shorter_data));

      const DataNDC initial2(initial_data.begin(), initial_data.end(), lookup5, pool5);
      CHECK((initial <= initial2) == (initial_data <= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      LookupNDC lookup3;
      PoolNDC   pool3;

      LookupNDC lookup4;
      PoolNDC   pool4;

      const DataNDC less(less_data.begin(), less_data.end(), lookup1, pool1);
      const DataNDC initial(initial_data.begin(), initial_data.end(), lookup2, pool2);

      CHECK((less > initial) == (less_data > initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end(), lookup3, pool3);

      CHECK((greater > initial) == (greater_data > initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end(), lookup4, pool4);

      CHECK((shorter > initial) == (shorter_data > initial_data));
      CHECK((initial > shorter) == (initial_data > shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than_or_equal)
    {
      LookupNDC lookup1;
      PoolNDC   pool1;

      LookupNDC lookup2;
      PoolNDC   pool2;

      LookupNDC lookup3;
      PoolNDC   pool3;

      LookupNDC lookup4;
      PoolNDC   pool4;

      LookupNDC lookup5;
      PoolNDC   pool5;

      const DataNDC less(less_data.begin(), less_data.end(), lookup1, pool1);
      const DataNDC initial(initial_data.begin(), initial_data.end(), lookup2, pool2);

      CHECK((less >= initial) == (less_data >= initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end(), lookup3, pool3);

      CHECK((greater >= initial) == (greater_data >= initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end(), lookup4, pool4);

      CHECK((shorter >= initial) == (shorter_data >= initial_data));
      CHECK((initial >= shorter) == (initial_data > shorter_data));

      const DataNDC initial2(initial_data.begin(), initial_data.end(), lookup5, pool5);
      CHECK((initial >= initial2) == (initial_data >= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_default_order_from_unordered_std_sort)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      std::sort(data.begin(), data.end());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 ordered_data.begin());

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_default_order_from_unordered_std_stable_sort)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      std::stable_sort(data.begin(), data.end());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_default_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_default_order_from_ordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(ordered_data.begin(), ordered_data.end(), lookup, pool);

      data.sort();

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 ordered_data.begin());

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_default_order_from_reverse_ordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(ordered_data.rbegin(), ordered_data.rend(), lookup, pool);

      data.sort();

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 ordered_data.begin());

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_default_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.sort();

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 ordered_data.begin());

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_greater_order_from_reverse_ordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(ordered_data.rbegin(), ordered_data.rend(), lookup, pool);

      data.sort(std::greater<NDC>());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 ordered_data.rbegin());

      CHECK(is_equal);
      CHECK(data.is_sorted(std::greater<NDC>()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_greater_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.sort(std::greater<NDC>());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 ordered_data.rbegin());

      CHECK(is_equal);
      CHECK(data.is_sorted(std::greater<NDC>()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_default_partial_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.sort(data.begin() + 2, data.end() - 2);

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 part_ordered_data.begin());

      CHECK(is_equal);
      CHECK((data.is_sorted(data.cbegin() + 2, data.cend() - 2)));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_greater_partial_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.sort(data.begin() + 2, data.end() - 2, std::greater<NDC>());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 part_reverse_ordered_data.begin());

      CHECK(is_equal);
      CHECK((data.is_sorted(data.begin() + 2, data.end() - 2, std::greater<NDC>())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_default_order_from_ordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(stable_default_ordered_data.begin(), stable_default_ordered_data.end(), lookup, pool);

      data.stable_sort();

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_default_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_default_order_from_reverse_ordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(stable_default_ordered_data.rbegin(), stable_default_ordered_data.rend(), lookup, pool);

      data.stable_sort();

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_reverse_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_default_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.stable_sort();

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_default_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK(data.is_sorted());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_greater_order_from_reverse_ordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(stable_default_ordered_data.rbegin(), stable_default_ordered_data.rend(), lookup, pool);

      data.stable_sort(std::greater<NDC>());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_greater_reverse_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK(data.is_sorted(std::greater<NDC>()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_greater_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.stable_sort(std::greater<NDC>());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_greater_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK(data.is_sorted(std::greater<NDC>()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_default_partial_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.stable_sort(data.begin() + 2, data.end() - 2);

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_part_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK((data.is_sorted(data.cbegin() + 2, data.cend() - 2)));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort_greater_partial_order_from_unordered)
    {
      LookupNDC lookup;
      PoolNDC   pool;

      DataNDC data(unordered_data.begin(), unordered_data.end(), lookup, pool);

      data.stable_sort(data.begin() + 2, data.end() - 2, std::greater<NDC>());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 stable_part_greater_ordered_data.begin(),
                                 NDC::are_identical);

      CHECK(is_equal);
      CHECK((data.is_sorted(data.begin() + 2, data.end() - 2, std::greater<NDC>())));
    }
  };
}
