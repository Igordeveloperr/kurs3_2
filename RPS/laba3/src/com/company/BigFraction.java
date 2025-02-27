package com.company;

import java.math.BigInteger;

public class BigFraction {

    /**
     * Extra variables
     */
    BigInteger numerator = BigInteger.valueOf(788);
    BigInteger denominator = BigInteger.valueOf(788);


    /**
     * Constructor for input values
     * @param numerator Numerator of the first fraction
     * @param denominator Denominator of the first fraction
     */
    public BigFraction(BigInteger numerator, BigInteger denominator) {
        this.numerator = numerator;
        this.denominator = denominator;
    }

    /**
     * Function for adding two fractions
     * @param arg1 Numerator and denominator of two fractions
     * @return Result of adding two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction Addition(BigFraction arg1) {
        BigFractionCalculatorGUI.answer.numerator = (arg1.numerator.multiply(BigFractionCalculatorGUI.arg2.denominator)).add(BigFractionCalculatorGUI.arg2.numerator.multiply(arg1.denominator));
        BigFractionCalculatorGUI.answer.denominator = arg1.denominator.multiply(BigFractionCalculatorGUI.arg2.denominator);
        BigFraction last_answer = new BigFraction(BigFractionCalculatorGUI.answer.numerator, BigFractionCalculatorGUI.answer.denominator);
        last_answer = Reduction(last_answer);
        return last_answer;
    }

    /**
     * Function for subtracting two fractions
     * @param arg1 Numerator and denominator of two fractions
     * @return Result of subtracting two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction Subtraction(BigFraction arg1) {
        BigFractionCalculatorGUI.answer.numerator = (arg1.numerator.multiply(BigFractionCalculatorGUI.arg2.denominator)).subtract(BigFractionCalculatorGUI.arg2.numerator.multiply(arg1.denominator));
        BigFractionCalculatorGUI.answer.denominator = arg1.denominator.multiply(BigFractionCalculatorGUI.arg2.denominator);
        BigFraction last_answer = new BigFraction(BigFractionCalculatorGUI.answer.numerator, BigFractionCalculatorGUI.answer.denominator);
        last_answer = Reduction(last_answer);
        return last_answer;
    }


    /**
     * Function for multiplication two fractions
     * @param arg Numerator and denominator of two fractions
     * @return Result of multiplication two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction Multiplication(BigFraction arg) {
        BigFractionCalculatorGUI.answer.numerator = arg.numerator.multiply(BigFractionCalculatorGUI.arg2.numerator);
        BigFractionCalculatorGUI.answer.denominator = arg.denominator.multiply(BigFractionCalculatorGUI.arg2.denominator);
        BigFraction last_answer = new BigFraction(BigFractionCalculatorGUI.answer.numerator, BigFractionCalculatorGUI.answer.numerator);
        last_answer = Reduction(last_answer);
        return last_answer;
    }

    /**
     * Function for division two fractions
     * @param arg Numerator and denominator of two fractions
     * @return Result of division two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction Division(BigFraction arg) {
        BigFractionCalculatorGUI.answer.numerator = arg.numerator.multiply(BigFractionCalculatorGUI.arg2.denominator);
        BigFractionCalculatorGUI.answer.denominator = arg.denominator.multiply(BigFractionCalculatorGUI.arg2.numerator);
        BigFraction last_answer = new BigFraction(BigFractionCalculatorGUI.answer.numerator, BigFractionCalculatorGUI.answer.denominator);
        last_answer = Reduction(last_answer);
        return last_answer;
    }

    /**
     * Finding the maximum common divider
     * @param numerator Numerator of the number
     * @param denominator Denominator of the number
     * @return Maximum common divider
     */
    private BigInteger Checker(BigInteger numerator, BigInteger denominator) {

        /**
         * Variable for compare
         */
        int compare = denominator.compareTo(numerator);

        while (compare !=0) {
            compare =  denominator.compareTo(numerator);
            if (compare==1) denominator = denominator.subtract(numerator);
            else numerator = numerator.subtract(denominator);
        }
        return denominator;
    }

    /**
     * Function for reducing fractions
     * @param arg Numerator and denominator of two fractions
     * @return Reduced numerators and denominators of operands and result
     */
    public BigFraction Reduction(BigFraction arg) {
        /**
         * Variable for additional calculations
         */
        BigInteger dop;

        dop = Checker(BigFractionCalculatorGUI.answer.numerator, BigFractionCalculatorGUI.answer.denominator);
        BigFractionCalculatorGUI.answer.numerator = BigFractionCalculatorGUI.answer.numerator.divide(dop);
        BigFractionCalculatorGUI.answer.denominator = BigFractionCalculatorGUI.answer.denominator.divide(dop);

        return BigFractionCalculatorGUI.answer;
    }

}
