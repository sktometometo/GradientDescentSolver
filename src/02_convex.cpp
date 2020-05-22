// standard library
#include <iostream>
// boost
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/program_options.hpp>
// Eigen
#include <Eigen/Dense>
//
#include "gradient_descent_solver.h"

double fn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b )
{
    Eigen::VectorXd t = ( b - A * x );
    return t.dot( t );
}

Eigen::VectorXd dfn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b )
{
    return - 2 * A.transpose() * b + 2 * A.transpose() * A * x;
}

double errorn( Eigen::VectorXd x, Eigen::VectorXd x_star, Eigen::MatrixXd A, Eigen::VectorXd b )
{
    return std::abs( fn( x, A, b ) - fn( x_star, A, b ) );
}

int main( int argc, char **argv )
{
    boost::program_options::options_description opt("option");
    opt.add_options()
            ("help,h", "shot help")
            ("degree,d", boost::program_options::value<int>(), "size of problem");
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store( boost::program_options::parse_command_line( argc, argv, opt ), vm );
    } catch ( const boost::program_options::error_with_option_name& e ) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    boost::program_options::notify( vm );

    int degree = 10;
    if ( vm.count("help") || !vm.count("degree") ) {
        std::cout << opt << std::endl;
        return 0;
    } else {
        try {
            degree = vm["degree"].as<int>();
        } catch ( const boost::bad_any_cast& e ) {
            std::cout << e.what() << std::endl;
            return 0;
        }
    }


    Eigen::MatrixXd A = Eigen::MatrixXd::Random(degree,degree) * 100;
    Eigen::VectorXd omega_hat = Eigen::VectorXd::Ones(degree);
    Eigen::VectorXd eps = Eigen::VectorXd::Random(degree) * 10;
    Eigen::VectorXd b = A * omega_hat + eps;
    Eigen::VectorXd x_start = Eigen::VectorXd::Random(degree) * 100;

    std::cout << std::fixed;

    GradientDescentSolver<Eigen::VectorXd> solver( false );
    solver.setf( boost::bind( fn, _1, A, b ) );
    solver.setdf( boost::bind( dfn, _1, A, b ) );
    solver.seterror( boost::bind( errorn, _1, omega_hat, A, b ) );
    Eigen::VectorXd x_target = solver.solve( x_start, 1.0 );

    return 0;
}
