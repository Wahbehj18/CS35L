#!/usr/bin/python

"""

Jacob Wahbeh
105114897
shuf.py

"""

import random, sys
from optparse import OptionParser


def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    #arg -n headcount
    parser.add_option("-n", "--head-count",
                      action="store", dest="headCount", default=None,
                      help="""Output at most COUNT lines. By default, 
                      all input lines are output""")

    #arg -i inputrange
    parser.add_option("-i", "--input-range",
                      action="store", dest="inputRange", default=None,
                      help="Treat each number LO through HI as an input line")
    #arg -r repeat
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="Repeat output values")
    options, args = parser.parse_args(sys.argv[1:])

    if options.headCount:
        try:
            num = int(options.headCount)
        except:
            parser.error("invalid line count: '" + options.headCount + "'")
        if num < 0:
            parser.error("invalid line count: '" + options.headCount + "'")

    inLines = []
    iRange = options.inputRange
    if iRange:
        try:
            loHi = iRange.split("-")
            lo = int(loHi[0])
            hi = int(loHi[1])
        except:
            parser.error("invalid input range: '" + iRange + "'")
        if hi < lo:
            parser.error("invalid input range: '" + iRange + "'")
        if len(args) != 0:
            parser.error("extra operand '" + args[0] + "'")

        for x in range(lo,hi+1):
            inLines.append(str(x) + "\n")

    elif len(args) == 0 or (args[0] == "-" and len(args) == 1):
        inLines = sys.stdin.readlines()
       
    elif len(args) > 1:
        parser.error("extra operand '" + args[1] + "'")
    else:
        try:
            file1 = open(args[0])
            inLines = file1.readlines()
            file1.close()
        except:
            parser.error(args[0] +" : No such file or directory")
        
    
    repeat = options.repeat

    if not options.headCount or int(options.headCount) > len(inLines):
        num = len(inLines)
    
    if not repeat:
        random.shuffle(inLines)
        for i in range(num):
            sys.stdout.write(inLines[i])

    if repeat and options.headCount:
        for j in range(int(options.headCount)):
            random.shuffle(inLines)
            sys.stdout.write(inLines[0])
    if num == 0 and repeat:
        sys.stdout.write("no lines to repeat\n")
    if repeat and not options.headCount and num > 0:
        while repeat:
            random.shuffle(inLines)
            sys.stdout.write(inLines[0])
        
    
if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        # do nothing here
        pass
