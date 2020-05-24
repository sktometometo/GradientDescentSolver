// standard library
#include <iostream>
#include <fstream>
#include <regex>
// boost
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp> // is_any_of
#include <boost/algorithm/string/split.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/program_options.hpp>
// Eigen
#include <Eigen/Dense>
//
#include "gradient_descent_solver.h"

double fn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b, double c )
{
    double t = ( x.transpose() * A * x + b.dot( x ) + c );
    return t * t;
}

Eigen::VectorXd dfn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b, double c )
{
    return 2 * ( x.transpose() * A * x + b.dot( x ) + c ) * ( 2 * A.transpose() * x + b );
}

double errorn( Eigen::VectorXd x, Eigen::MatrixXd A, Eigen::VectorXd b, double c  )
{
    return std::abs( dfn( x, A, b, c ).norm() );
}

bool loadParam( std::string filename, int& degree, Eigen::MatrixXd& A, Eigen::VectorXd& b, double& c, Eigen::VectorXd& x_start )
{
    std::ifstream ifs( filename );
    std::string str;

    if ( ifs.fail() ) {
        return false;
    } else {
        std::getline( ifs, str ); //
        std::getline( ifs, str ); //
        degree = std::stoi( str );

        A = Eigen::MatrixXd::Identity(degree,degree);
        b = Eigen::VectorXd::Ones(degree);
        c = 0.0;
        x_start = b;

        std::getline( ifs, str ); //
        for ( int i=0; i<degree; i++ ) {
            std::getline( ifs, str ); //
            boost::algorithm::trim( str );
            str = std::regex_replace( str, std::regex(" +"), " " );
            std::vector<std::string> result;
            boost::algorithm::split( result, str, boost::is_any_of(" ") );
            for ( int j=0; j<degree; j++ ) {
                A(i,j) = std::stod( result[j] );
            }
        }

        std::getline( ifs, str ); //
        for ( int i=0; i<degree; i++ ) {
            std::getline( ifs, str ); //
            boost::algorithm::trim( str );
            b(i) = std::stod( str );
        }

        std::getline( ifs, str ); //
        std::getline( ifs, str ); //
        c = std::stod( str );

        std::getline( ifs, str ); //
        for ( int i=0; i<degree; i++ ) {
            std::getline( ifs, str ); //
            boost::algorithm::trim( str );
            x_start(i) = std::stod( str );
        }

        return true;
    }
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
            ("printparam", boost::program_options::value<bool>()->default_value(false), "print parameter" )
            ("loadparamfile", boost::program_options::value<std::string>(), "" );
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store( boost::program_options::parse_command_line( argc, argv, opt ), vm );
    } catch ( const boost::program_options::error_with_option_name& e ) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    boost::program_options::notify( vm );

    std::srand(std::time(0));

    int accel;
    int debug;
    int degree;
    double error;
    bool printparam;
    Eigen::MatrixXd A;
    Eigen::VectorXd b, x_start;
    double c;
    if ( vm.count("help") || ( !vm.count("loadparamfile") and !vm.count("degree") ) || !vm.count("error") ) {
        std::cout << opt << std::endl;
        return 0;
    } else if ( vm.count("loadparamfile") ) {
        std::string filename;
        try {
            accel = vm["accel"].as<int>();
            debug = vm["debug"].as<int>();
            error = vm["error"].as<double>();
            printparam = vm["printparam"].as<bool>();
            filename = vm["loadparamfile"].as<std::string>();
        } catch ( const boost::bad_any_cast& e ) {
            std::cout << e.what() << std::endl;
            return 0;
        }
        if ( not loadParam( filename, degree, A, b, c, x_start ) ) {
            std::cout << opt << std::endl;
            return 0;
        }
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
        double coef = 4.0 / degree;
        A = Eigen::MatrixXd::Random(degree,degree) * 3 * coef;
        b = Eigen::VectorXd::Random(degree) * 3 * coef;
        c = std::rand() * 3.0 * coef / RAND_MAX;
        x_start = Eigen::VectorXd::Random(degree) * 10;
    }

    std::cout << std::fixed;

    if ( printparam ) {
        std::cout << "degree:" << std::endl;
        std::cout << degree << std::endl;
        std::cout << "A:" << std::endl;
        std::cout << A << std::endl;
        std::cout << "b:" << std::endl;
        std::cout << b << std::endl;
        std::cout << "c:" << std::endl;
        std::cout << c << std::endl;
        std::cout << "x_start:" << std::endl;
        std::cout << x_start << std::endl;
    }

    GradientDescentSolver<Eigen::VectorXd> solver( debug );
    solver.setf( boost::bind( fn, _1, A, b, c ) );
    solver.setdf( boost::bind( dfn, _1, A, b, c ) );
    solver.seterror( boost::bind( errorn, _1, A, b, c ) );
    std::vector<Eigen::VectorXd> trajectory;
    Eigen::VectorXd x_target = solver.solve( x_start, trajectory, error, accel );

    if ( isnan( x_target.norm() ) ) {
        std::cout << "diverged." << std::endl;
    }

    return 0;
}
