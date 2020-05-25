#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import numpy as np
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys
import re

def fn( x, y, A, b ):
    t0 = b[0] - ( A[0,0] * x + A[0,1] * y )
    t1 = b[1] - ( A[1,0] * x + A[1,1] * y )
    return t0 ** 2 + t1 ** 2

def readParameterFromFile( filename ):

    A = np.array( [[0,0],[0,0]] )
    b = np.array( [0,0] )
    x_start = np.array( [0,0] )

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
        x_start[0] = float( f.readline().lstrip(' ').strip('\n') )
        x_start[1] = float( f.readline().lstrip(' ').strip('\n') )

    return A, b, x_start

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
    parser.add_argument('-p','--parameter',type=str,help='parameter file to read')
    parser.add_argument('-t','--trajectory',type=str,help='trajectory file')
    args = parser.parse_args()

    if not args.parameter or not args.trajectory:
        print('please specify parameter and trajectory')
        sys.exit()

    A, b, x_start = readParameterFromFile( args.parameter )
    xs, ys = readTrajectory( args.trajectory )

    x_star = np.dot( np.linalg.inv(A), b )

    plotrange = math.sqrt( np.max(np.abs(xs - x_star[0])) ** 2 + np.max(np.abs(ys - x_star[1])) ** 2 )
    x = np.linspace( x_star[0] - plotrange, x_star[0] + plotrange, 1024 )
    y = np.linspace( x_star[1] - plotrange, x_star[1] + plotrange, 1024 )

    X, Y = np.meshgrid( x, y )
    Z = fn( X, Y, A, b )

    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.plot_surface( X, Y, Z, cmap = 'summer' )
    ax.contour( X, Y, Z, colors='black', offset=-1 )
    ax.plot( xs, ys, zs=0, zdir='z' )
    plt.show()

if __name__ == '__main__':
    main()

