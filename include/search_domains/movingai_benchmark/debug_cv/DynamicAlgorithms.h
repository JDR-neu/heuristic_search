/* By downloading, copying, installing or using the software you agree to this license.
 * 
 *                           License Agreement
 *                      For heuristic_search library
 *
 * Copyright (c) 2016, 
 * Maciej Przybylski <maciej.przybylski@mchtr.pw.edu.pl>, 
 * Warsaw University of Technology.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holders nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#ifndef DYNAMICALGORITHMS_H
#define DYNAMICALGORITHMS_H

#include <functional>

#include "heuristic_search/loggers/Log.h"
#include "search_domains/movingai_benchmark/benchmark/DynamicExperiment.h"

namespace movingai_benchmark{
namespace debug_cv{

struct VisualizationConfig
{
    VisualizationConfig()
    {
        scale = 1.0;
        delay = 5;
        hide_text = false;
        
    }
    
    double scale;
    int delay;
    bool hide_text;
};

heuristic_search::loggers::Log runAStarForDynamicExperiment(
        movingai_benchmark::benchmark::DynamicExperiment const& experiment, 
        heuristic_search::SearchingDirection direction, 
        std::string const& name, VisualizationConfig config);

std::function<heuristic_search::loggers::Log(
        movingai_benchmark::benchmark::DynamicExperiment const& )> 
    getDynamicAlgorithm(std::string const& name, VisualizationConfig config);

}//namespace debug_cv
}//namespace movingai_benchmark

#endif /* DYNAMICALGORITHMS_H */

