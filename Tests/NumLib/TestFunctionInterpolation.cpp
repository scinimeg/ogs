/**
 * \copyright
 * Copyright (c) 2012-2015, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/LICENSE.txt
 */

#include <gtest/gtest.h>

#include <array>

#include "NumLib/Function/Interpolation.h"

#include "NumLib/Fem/FiniteElement/TemplateIsoparametric.h"
#include "NumLib/Fem/ShapeFunction/ShapeLine2.h"

#include "NumLib/Fem/ShapeMatrixPolicy.h"

#include "MeshLib/Elements/Element.h"

#include "NumLib/Fem/Integration/GaussIntegrationPolicy.h"


TEST(NumLibFunctionInterpolationTest, TwoVariablesTwoNodes)
{
    double variable1 = 0.0;
    double variable2 = 0.0;
    std::array<double*, 2> interpolated_values = { &variable1, &variable2 };

    const std::array<double, 4> nodal_values = {
        0.0, 1.0, // for variable1
        -1.0, 1.0  // for variable2
    };

    const std::array<double, 2> shape_matrix = { 0.25, 0.75 };

    NumLib::shapeFunctionInterpolate(nodal_values, shape_matrix,
                                     interpolated_values);

    ASSERT_EQ(0.75, variable1);
    ASSERT_EQ(0.5,  variable2);
}


TEST(NumLibFunctionInterpolationTest, Linear1DElement)
{
    // typedefs
    using ShapeFunction = NumLib::ShapeLine2;
    using ShapeMatricesType = ShapeMatrixPolicyType<ShapeFunction, 1u>;

    using FemType = NumLib::TemplateIsoparametric<ShapeFunction, ShapeMatricesType>;

    using IntegrationMethod = NumLib::GaussIntegrationPolicy<
        ShapeFunction::MeshElement>::IntegrationMethod;

    // set up mesh element
    auto pt_a = MeshLib::Node({ 0.0, 0.0, 0.0 }, 0);
    auto pt_b = MeshLib::Node({ 1.0, 0.0, 0.0 }, 0);

    auto const element = MeshLib::Line(std::array<MeshLib::Node*, 2>{
                                           &pt_a, &pt_b
                                       }, 0u, 0u);

    // set up shape function
    FemType finite_element(*static_cast<const ShapeFunction::MeshElement*>(&element));

    const unsigned integration_order = 2;
    IntegrationMethod integration_method(integration_order);

    ShapeMatricesType::ShapeMatrices shape_matrix;

    finite_element.computeShapeFunctions(
            integration_method.getWeightedPoint(0).getCoords(),
            shape_matrix);
    ASSERT_EQ(2, shape_matrix.N.size());

    // actual test
    double variable1 = 0.0;
    double variable2 = 0.0;
    std::array<double*, 2> interpolated_values = { &variable1, &variable2 };

    const std::array<double, 4> nodal_values = {
        0.0, 1.0, // for variable1
        -1.0, 1.0  // for variable2
    };

    NumLib::shapeFunctionInterpolate(nodal_values, shape_matrix.N,
                                     interpolated_values);

    const double n0 = shape_matrix.N[0];
    const double n1 = shape_matrix.N[1];

    ASSERT_EQ( 0.0 * n0 + 1.0 * n1, variable1);
    ASSERT_EQ(-1.0 * n0 + 1.0 * n1, variable2);
}


