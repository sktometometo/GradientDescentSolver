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

double fn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b, double lambda )
{
    Eigen::VectorXd t = ( b - A * x );
    return t.dot( t ) + lambda * x.dot( x );
}

Eigen::VectorXd dfn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b, double lambda )
{
    return 2 * ( A.transpose() * A - lambda * Eigen::MatrixXd::Identity( A.rows(), A.cols() ) ) * x - 2 * A.transpose() * b;
}

double errorn( Eigen::VectorXd x, Eigen::VectorXd x_star, Eigen::MatrixXd A, Eigen::VectorXd b, double lambda )
{
    return std::abs( fn( x, A, b, lambda ) - fn( x_star, A, b, lambda ) );
}

int main( int argc, char **argv )
{
    boost::program_options::options_description opt("option");
    opt.add_options()
            ("help,h", "shot help")
            ("debug", boost::program_options::value<bool>()->default_value(false), "print trajectory" )
            ("degree,d", boost::program_options::value<int>(), "size of problem")
            ("error,e", boost::program_options::value<double>(), "threshold of error")
            ("lambda,l", boost::program_options::value<double>(), "lambda");
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store( boost::program_options::parse_command_line( argc, argv, opt ), vm );
    } catch ( const boost::program_options::error_with_option_name& e ) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    boost::program_options::notify( vm );

    bool debug;
    int degree;
    double error;
    double lambda;
    if ( vm.count("help") || !vm.count("degree") || !vm.count("error") || !vm.count("lambda") ) {
        std::cout << opt << std::endl;
        return 0;
    } else {
        try {
            debug = vm["debug"].as<bool>();
            degree = vm["degree"].as<int>();
            error = vm["error"].as<double>();
            lambda = vm["lambda"].as<double>();
        } catch ( const boost::bad_any_cast& e ) {
            std::cout << e.what() << std::endl;
            return 0;
        }
    }

    srand((unsigned int) time(0));

    Eigen::MatrixXd A = Eigen::MatrixXd::Random(degree,degree) * 100;
    Eigen::VectorXd omega_hat = Eigen::VectorXd::Ones(degree);
    Eigen::VectorXd eps = Eigen::VectorXd::Random(degree) * 10;
    Eigen::VectorXd b = A * omega_hat + eps;
    Eigen::VectorXd x_star = A.inverse() * b;
    Eigen::VectorXd x_start = Eigen::VectorXd::Random(degree) * 100;

    std::cout << std::fixed;

    GradientDescentSolver<Eigen::VectorXd> solver( debug );
    solver.setf( boost::bind( fn, _1, A, b, lambda ) );
    solver.setdf( boost::bind( dfn, _1, A, b, lambda ) );
    solver.seterror( boost::bind( errorn, _1, x_star, A, b, lambda ) );
    std::vector<Eigen::VectorXd> trajectory;
    Eigen::VectorXd x_target = solver.solve( x_start, trajectory, error );

    return 0;
}
