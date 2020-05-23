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
    return 2 * ( ( b - A * x ).transpose() * ( - 1 * A ) ).transpose();
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
            ("accel,a", boost::program_options::value<int>()->default_value(0), "acceleration method" )
            ("debug", boost::program_options::value<int>()->default_value(0), "debug mode" )
            ("degree,d", boost::program_options::value<int>(), "size of problem")
            ("error,e", boost::program_options::value<double>(), "threshold of error")
            ("printparam", boost::program_options::value<bool>()->default_value(false), "print parameter" );
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store( boost::program_options::parse_command_line( argc, argv, opt ), vm );
    } catch ( const boost::program_options::error_with_option_name& e ) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    boost::program_options::notify( vm );

    int accel;
    int debug;
    int degree;
    double error;
    bool printparam;
    if ( vm.count("help") || !vm.count("degree") || !vm.count("error") ) {
        std::cout << opt << std::endl;
        return 0;
    } else {
        try {
            accel = vm["accel"].as<int>();
            debug = vm["debug"].as<int>();
            degree = vm["degree"].as<int>();
            error = vm["error"].as<double>();
            printparam = vm["printparam"].as<bool>();
        } catch ( const boost::bad_any_cast& e ) {
            std::cout << e.what() << std::endl;
            return 0;
        }
    }

    std::srand(std::time(0));

    double coef = 4.0 / degree;

    Eigen::MatrixXd A = Eigen::MatrixXd::Random(degree,degree) * 3 * coef;
    A = A.transpose() * A;
    Eigen::VectorXd omega_hat = Eigen::VectorXd::Ones(degree);
    Eigen::VectorXd eps = Eigen::VectorXd::Random(degree);
    Eigen::VectorXd b = A * omega_hat + eps;
    Eigen::VectorXd x_star = A.inverse() * b;
    Eigen::VectorXd x_start = Eigen::VectorXd::Random(degree) * 10 * coef;

    std::cout << std::fixed;

    if ( printparam ) {
        std::cout << "A:" << std::endl;
        std::cout << A << std::endl;
        std::cout << "b:" << std::endl;
        std::cout << b << std::endl;
        std::cout << "x_start:" << std::endl;
        std::cout << x_start << std::endl;
    }

    GradientDescentSolver<Eigen::VectorXd> solver( debug );
    solver.setf( boost::bind( fn, _1, A, b ) );
    solver.setdf( boost::bind( dfn, _1, A, b ) );
    solver.seterror( boost::bind( errorn, _1, x_star, A, b ) );
    std::vector<Eigen::VectorXd> trajectory;
    Eigen::VectorXd x_target = solver.solve( x_start, trajectory, error, accel );

    if ( isnan( x_target.norm() ) ) {
        std::cout << "diverged." << std::endl;
    }

    return 0;
}
