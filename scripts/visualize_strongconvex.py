#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys
import re

def fn( x, y, A, b, l ):
    t0 = b[0] - ( A[0,0] * x + A[0,1] * y )
    t1 = b[1] - ( A[1,0] * x + A[1,1] * y )
    return t0 ** 2 + t1 ** 2 + l * ( x ** 2 + y ** 2 )

def readParameterInteractive():

    A = np.array( [[0,0],[0,0]] )
    b = np.array( [0,0] )
    x_start = np.array( [0,0] )
    l = 0.0

    A[0,0] = input( 'A[0,0]:' )
    A[0,1] = input( 'A[0,1]:' )
    A[1,0] = input( 'A[1,0]:' )
    A[1,1] = input( 'A[1,1]:' )
    b[0] = input( 'b[0]:' )
    b[1] = input( 'b[1]:' )
    l = input( 'lambda:' )
    x_start[0] = input( 'x_start[0]:' )
    x_start[1] = input( 'x_start[1]:' )

    return A, b, l, x_start

def readParameterFromFile( filename ):
    
    A = np.array( [[0,0],[0,0]] )
    b = np.array( [0,0] )
    x_start = np.array( [0,0] )
    l = 0.0

    with open( filename ) as f:
        f.readline()
        f.readline()
        f.readline()
        [ A[0,0], A[0,1] ] = map( float, re.split( ' +', f.readline().lstrip(' ').strip('\n')) )
        [ A[1,0], A[1,1] ] = map( float, re.split( ' +', f.readline().lstrip(' ').strip('\n')) )
        f.readline()
        b[0] = float( f.readline().lstrip(' ').strip('\n') )
        b[1] = float( f.readline().lstrip(' ').strip('\n') )
        f.readline()
        l = float( f.readline().lstrip(' ').strip('\n') )
        f.readline()
        x_start[0] = float( f.readline().lstrip(' ').strip('\n') )
        x_start[1] = float( f.readline().lstrip(' ').strip('\n') )

    return A, b, l, x_start

def readTrajectory( filename ):

    xs = []
    ys = []

    with open( filename ) as f:
        while True:
            s = f.readline()
            if s == '':
                break
            l = s.strip(' ').strip('\n').split(',')
            xs.append( float(l[3]) )
            ys.append( float(l[4]) )

    xs = np.array( xs )
    ys = np.array( ys )

    return xs, ys

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('-i','--interactive',help='input parameter interactively',action='store_true')
    parser.add_argument('-f','--file',type=str,help='parameter file to read')
    parser.add_argument('--trajectory',type=str,help='trajectory file')
    args = parser.parse_args()

    if args.interactive:
        A, b, l, x_start = readParameterInteractive()
    else:
        if not args.file:
            print('please specify file')
            sys.exit()
        filename = args.file
        A, b, l, x_start = readParameterFromFile( filename )

    x_star = 2 * np.dot( np.dot( np.linalg.inv( 2 * np.dot( A.transpose(), A ) + l * np.eye( A.shape[0] ) ), A.transpose() ), b )

    if args.trajectory:
        xs, ys = readTrajectory( args.trajectory )
        plotrangemax = max( np.max(np.abs(xs)), np.max(np.abs(ys)) )
        x = np.linspace( x_star - 1.5 * plotrangemax, x_star + 1.5 * plotrangemax, 1024 )
        y = np.linspace( x_star - 1.5 * plotrangemax, x_star + 1.5 * plotrangemax, 1024 )
    else:
        plotrangemax = max( abs( x_start[0] ), abs( x_start[1] ) )
        x = np.linspace( x_star - 1.5 * plotrangemax, x_star + 1.5 * plotrangemax, 1024 )
        y = np.linspace( x_star - 1.5 * plotrangemax, x_star + 1.5 * plotrangemax, 1024 )

    X, Y = np.meshgrid( x, y )

    Z = fn( X, Y, A, b, l )

    fig = plt.figure()
    ax = fig.gca(projection='3d')

    ax.plot_surface( X, Y, Z, cmap = 'summer' )
    ax.contour( X, Y, Z, colors='black', offset=-1 )

    if args.trajectory:
        ax.plot( xs, ys, zs=0, zdir='z' )

    plt.show()

if __name__ == '__main__':
    main()

