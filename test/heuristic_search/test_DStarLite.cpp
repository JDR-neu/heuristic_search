/* heuristic_search library
 *
 * Copyright (c) 2016, 
 * Maciej Przybylski <maciej.przybylski@mchtr.pw.edu.pl>, 
 * Warsaw University of Technology.
 * All rights reserved.
 *  
 */

#include <gtest/gtest.h>

#include "heuristic_search/SearchLoop.h"
#include "heuristic_search/StdOpenList.h"
#include "heuristic_search/StdSearchSpace.h"
#include "heuristic_search/SearchingAlgorithm.h"
#include "heuristic_search/HeuristicSearch.h"
#include "heuristic_search/DStarLite.h"
#include "heuristic_search/DStarMain.h"

#include "../heuristic_search/test_TestDomain.h"

namespace test_heuristic_search{


typedef heuristic_search::SearchAlgorithmBegin<            
            heuristic_search::DStarMain<
            heuristic_search::SearchLoop<
            heuristic_search::DStarLite<
            heuristic_search::HeuristicSearch<
            heuristic_search::StdOpenList<
            heuristic_search::StdSearchSpace<
        heuristic_search::SearchAlgorithmEnd<TestDomain> > > > > > > >   DStarLiteTestAlgorithm_t;
    
TEST(test_DStarLite, search_reinitialize_start)
{        
    TestDomain domain;
    
    DStarLiteTestAlgorithm_t::Algorithm_t algorithm(domain, 
            heuristic_search::SearchingDirection::Backward, true);
       
    ASSERT_FALSE(algorithm.initialized);
    ASSERT_FALSE(algorithm.finished);
    ASSERT_FALSE(algorithm.found);

    ASSERT_TRUE(algorithm.search({1},{5}));
    ASSERT_TRUE(algorithm.start_node->visited);
    ASSERT_TRUE(algorithm.goal_node->visited);

    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    std::vector<std::pair<TestDomain::StateActionState, TestDomain::Cost> > updated_actions;
    algorithm.reinitialize({0}, updated_actions);
    ASSERT_TRUE(algorithm.search());    
    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    auto path = algorithm.getStatePath();
    
    ASSERT_EQ(6, path.size());
    
    EXPECT_EQ(TestDomain::State({0}),path[0]);
    EXPECT_EQ(TestDomain::State({1}),path[1]);
    EXPECT_EQ(TestDomain::State({2}),path[2]);
    EXPECT_EQ(TestDomain::State({3}),path[3]);
    EXPECT_EQ(TestDomain::State({4}),path[4]);
    EXPECT_EQ(TestDomain::State({5}),path[5]);
    
}

TEST(test_DStarLite, search_reinitialize_start_in_range)
{        
    TestDomain domain;
    
    DStarLiteTestAlgorithm_t::Algorithm_t algorithm(domain, 
            heuristic_search::SearchingDirection::Backward, true);
       
    ASSERT_FALSE(algorithm.initialized);
    ASSERT_FALSE(algorithm.finished);
    ASSERT_FALSE(algorithm.found);

    ASSERT_TRUE(algorithm.search({0},{5}));
    ASSERT_TRUE(algorithm.start_node->visited);
    ASSERT_TRUE(algorithm.goal_node->visited);

    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    std::vector<std::pair<TestDomain::StateActionState, TestDomain::Cost> > updated_actions;
    algorithm.reinitialize({1}, updated_actions);
    ASSERT_TRUE(algorithm.search());    
    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    auto path = algorithm.getStatePath();
    
    ASSERT_EQ(5, path.size());
    
    EXPECT_EQ(TestDomain::State({1}),path[0]);
    EXPECT_EQ(TestDomain::State({2}),path[1]);
    EXPECT_EQ(TestDomain::State({3}),path[2]);
    EXPECT_EQ(TestDomain::State({4}),path[3]);
    EXPECT_EQ(TestDomain::State({5}),path[4]);

}

TEST(test_DStarLite, search_cost_increase)
{       
    typedef DStarLiteTestAlgorithm_t::Algorithm_t::Node_t Node_t;
    
    TestDomain domain;
    
    DStarLiteTestAlgorithm_t::Algorithm_t algorithm(domain, 
            heuristic_search::SearchingDirection::Backward, true);
       
    ASSERT_FALSE(algorithm.initialized);
    ASSERT_FALSE(algorithm.finished);
    ASSERT_FALSE(algorithm.found);

    ASSERT_TRUE(algorithm.search({0},{5}));
    ASSERT_TRUE(algorithm.start_node->visited);
    ASSERT_TRUE(algorithm.goal_node->visited);

    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    Node_t *node_0 = algorithm.search_space->getNode({0});    
    EXPECT_TRUE(node_0->visited);
    EXPECT_FALSE(node_0->open);
    EXPECT_TRUE(node_0->closed);
    EXPECT_EQ(5*TestDomain::costFactor(), node_0->rhs);
    EXPECT_EQ(node_0->rhs, node_0->g);
    
    TestDomain::StateActionState updated_action({2},{10*TestDomain::costFactor()},{3});
    domain.updateAction(updated_action);
    std::vector<std::pair<TestDomain::StateActionState, TestDomain::Cost> > updated_actions;
    updated_actions.push_back(std::make_pair(updated_action, 1*TestDomain::costFactor()));
    
    algorithm.reinitialize({1}, updated_actions);
        
    Node_t *node_2 = algorithm.search_space->getNode({2});    
    EXPECT_TRUE(node_2->visited);
    EXPECT_TRUE(node_2->open);
    EXPECT_FALSE(node_2->closed);
    
    Node_t *node_3 = algorithm.search_space->getNode({3});    
    EXPECT_TRUE(node_3->visited);
    EXPECT_FALSE(node_3->open);
    EXPECT_TRUE(node_3->closed);
    
    Node_t *node_1 = algorithm.search_space->getNode({1});    
    EXPECT_TRUE(node_1->visited);
    EXPECT_FALSE(node_1->open);
    EXPECT_TRUE(node_1->closed);
    
    EXPECT_EQ(node_1->g + 1*TestDomain::costFactor(), node_2->rhs);
    
    ASSERT_TRUE(algorithm.search());    
    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    EXPECT_EQ(6*TestDomain::costFactor(), node_1->rhs);
        
    auto path = algorithm.getStatePath();
    
    ASSERT_EQ(5, path.size());
    
    EXPECT_EQ(TestDomain::State({1}),path[0]);
    EXPECT_EQ(TestDomain::State({6}),path[1]);
    EXPECT_EQ(TestDomain::State({7}),path[2]);
    EXPECT_EQ(TestDomain::State({4}),path[3]);
    EXPECT_EQ(TestDomain::State({5}),path[4]);

}

TEST(test_DStarLite, search_cost_decrease)
{       
    typedef DStarLiteTestAlgorithm_t::Algorithm_t::Node_t Node_t;
    
    TestDomain domain;
    
    DStarLiteTestAlgorithm_t::Algorithm_t algorithm(domain, 
            heuristic_search::SearchingDirection::Backward, true);
        
    TestDomain::StateActionState updated_action({2},{10*TestDomain::costFactor()},{3});
    domain.updateAction(updated_action);
       
    ASSERT_FALSE(algorithm.initialized);
    ASSERT_FALSE(algorithm.finished);
    ASSERT_FALSE(algorithm.found);

    ASSERT_TRUE(algorithm.search({0},{5}));
    ASSERT_TRUE(algorithm.start_node->visited);
    ASSERT_TRUE(algorithm.goal_node->visited);

    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    auto old_path = algorithm.getStatePath();
    
    ASSERT_EQ(6, old_path.size());
    
    EXPECT_EQ(TestDomain::State({0}),old_path[0]);
    EXPECT_EQ(TestDomain::State({1}),old_path[1]);
    EXPECT_EQ(TestDomain::State({6}),old_path[2]);
    EXPECT_EQ(TestDomain::State({7}),old_path[3]);
    EXPECT_EQ(TestDomain::State({4}),old_path[4]);
    EXPECT_EQ(TestDomain::State({5}),old_path[5]);
    
    TestDomain::StateActionState updated_action_2({2},{1*TestDomain::costFactor()},{3});
    domain.updateAction(updated_action_2);
    std::vector<std::pair<TestDomain::StateActionState, TestDomain::Cost> > updated_actions;
    updated_actions.push_back(std::make_pair(updated_action_2, 10*TestDomain::costFactor()));
    
    algorithm.reinitialize({1}, updated_actions);
            
    Node_t *node_2 = algorithm.search_space->getNode({2});    
    EXPECT_TRUE(node_2->visited);
    EXPECT_TRUE(node_2->open);
    EXPECT_FALSE(node_2->closed);
        
    Node_t *node_3 = algorithm.search_space->getNode({3});    
    EXPECT_TRUE(node_3->visited);
    EXPECT_FALSE(node_3->open);
    EXPECT_TRUE(node_3->closed);
    
    ASSERT_TRUE(algorithm.search());    
    EXPECT_TRUE(algorithm.finished);
    EXPECT_TRUE(algorithm.found);
    
    auto path = algorithm.getStatePath();
    
    ASSERT_EQ(5, path.size());
    
    EXPECT_EQ(TestDomain::State({1}),path[0]);
    EXPECT_EQ(TestDomain::State({2}),path[1]);
    EXPECT_EQ(TestDomain::State({3}),path[2]);
    EXPECT_EQ(TestDomain::State({4}),path[3]);
    EXPECT_EQ(TestDomain::State({5}),path[4]);

}

}//namespace test_heuristic_search