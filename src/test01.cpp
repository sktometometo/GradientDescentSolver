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
    Eigen::Vector2d t = ( b - A * x );
    return t.dot( t );
}

Eigen::Vector2d dfn( Eigen::Vector2d x, Eigen::Matrix2d A, Eigen::Vector2d b )
{
    return - 2 * A.transpose() * b + 2 * A.transpose() * A * x;
}

double errorn( Eigen::Vector2d x, Eigen::Vector2d x_star, Eigen::Matrix2d A, Eigen::Vector2d b )
{
    return std::abs( fn( x, A, b ) - fn( x_star, A, b ) );
}

int main( int argc, char **argv )
{
    Eigen::Matrix2d A = Eigen::MatrixXd::Random(2,2) * 100;
    Eigen::Vector2d omega_hat = Eigen::VectorXd::Ones(2);
    Eigen::Vector2d eps = Eigen::VectorXd::Random(2) * 10;
    Eigen::Vector2d b = A * omega_hat + eps;
    Eigen::Vector2d x_start = Eigen::VectorXd::Random(2) * 100;

    /*
    Eigen::Matrix2d A;
    A << 2, 0,
         0, 2;
    Eigen::Vector2d b;
    b << 0, 0;
    Eigen::Vector2d x_start;
    x_start << 10, 10;
    */

    GradientDescentSolver<Eigen::Vector2d> solver( true );
    solver.setf( boost::bind( fn, _1, A, b ) );
    solver.setdf( boost::bind( dfn, _1, A, b ) );
    solver.seterror( boost::bind( errorn, _1, omega_hat, A, b ) );
    Eigen::Vector2d x_target = solver.solve( x_start, 0.1 );

    return 0;
}
