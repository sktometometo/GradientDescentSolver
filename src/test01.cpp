// standard library
#include <iostream>
// boost
#include <boost/bind.hpp>
#include <boost/function.hpp>
// Eigen
#include <Eigen/Dense>
//
#include "gradient_descent_solver.h"

double fn( Eigen::Vector2d x, Eigen::Matrix2d A, Eigen::Vector2d b )
{
    double t = ( b - A * x ).norm();
    return t * t;
}

int main( int argc, char **argv )
{
    Eigen::Matrix2d A = Eigen::MatrixXd::Random(2,2);
    Eigen::Vector2d omega_hat = Eigen::VectorXd::Ones(2);
    Eigen::Vector2d eps = Eigen::VectorXd::Random(2);
    Eigen::Vector2d b = A * omega_hat + eps;
    Eigen::Vector2d x_start = Eigen::VectorXd::Random(2);

    GradientDescentSolver<Eigen::Vector2d> solver( boost::bind( fn, _1, A, b ), true );
    Eigen::Vector2d x_target = solver.solve( x_start, 0.001 );

    return 0;
}
