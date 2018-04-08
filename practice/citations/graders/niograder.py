#!/usr/bin/env python2

## NIO GRADER FOR KATTIS

## IN GENERAL ABOUT KATTIS GRADERS
## From: http://www.problemarchive.org/wiki/index.php/Problem_Format#Graders
## Graders are programs that are given the sub-results of a test data group and aggregate a result for the group. They are provided in graders/ .
## For scoring problems, one common grader behaviour would be to always set the verdict to Accepted, with the score being the sum of scores of the items in the test group.

## When invoked the grader will get the judgement for test files or groups on stdin and is expected to produce an aggregate result on stdout.
## The grader should be possible to use as follows on the command line:
## ./grader [arguments] < judgeresults

## ABOUT THE NIO GRADER
## This particular grader is a scoring grader with four mutually exclusive modes, which in addition to a simple sum and ignore, is also capable of weighting subgroups of testcases differently by using the fractionAC mode with scaling. Mode is specified by command line arguments:
## ignore      - will output AC 0 if any of the inputs were accepted
## fractionAC  - will output the fraction of AC's to the total number of verdicts. Will output an AC verdict unless score is 0 and there was a non-AC verdict in input, then it will give first encountered error. By default, the output score is between 0 and 1 (two points float precision), but this can be scaled by giving a max score as an argument. Eg, if a result between 0 and 100 is desired, invoke the grader like below:
## $ python niograder.py fractionAC 100
## all         - will output AC only if all the testcases pass, logical AND. The score is 1 by default, but can be changed by providing a max score as an argument. Eg, to give 30 points if all are correct, invoke as follows:
## $ python niograder.py all 30
## sum         - will output the sum of the scores; will give AC verdict unless score is 0 and there was a non-AC verdict in input, then it will give first encountered error.

## One of the three modes above MUST be specified, or else the grader will
## output "None" and generate a judging error in Kattis

import sys

def grade():
    if "ignore" in sys.argv: return "AC", 0
        
    elif "fractionAC" in sys.argv:
        scaleto = getscale(sys.argv)
        correct = 0
        total = 0
        first_error = None
        for line in sys.stdin.readlines():
            verdict, score = line.split()
            if verdict != "AC" and not first_error: first_error = verdict
            total += 1
            correct += 1 if verdict == "AC" else 0
        if correct == 0 and first_error: return first_error, 0
        return "AC", (scaleto*correct/total)
        
    elif "all" in sys.argv:
        totscore = getscale(sys.argv)
        for line in sys.stdin.readlines():
            verdict, score = line.split()
            if verdict != "AC":
                return verdict, 0
        return "AC", totscore
        
    elif "sum" in sys.argv:
        totscore = 0
        first_error = None
        for line in sys.stdin.readlines():
            verdict, score = line.split()
            if verdict != "AC" and not first_error: first_error = verdict
            totscore += float(score)
                
        if totscore == 0 and first_error: return first_error, 0
        return "AC", totscore
    
    return "ERROR! N grading method found (ignore/fractionAC/all/sum)", -1


def is_num(arg):
    try:
        float(arg)
        return True
    except:
        return False
        
def getscale(args):
    for i in range(len(args)):
        if is_num(args[i]):
            return float(args[i])
    return 1.0 # Default

verdict, score = grade()
print("{} {:.2f}".format(verdict, score))