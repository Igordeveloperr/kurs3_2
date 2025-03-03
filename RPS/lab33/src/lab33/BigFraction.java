package lab33;

import java.math.BigInteger;

public class BigFraction {

    /**
     * Extra variables
     */
    BigInteger numerator = BigInteger.valueOf(1);
    BigInteger denominator = BigInteger.valueOf(1);
    int w1 = 1;
    int r = 13;
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
    public BigFraction addition(BigFraction arg) {
        BigFractionCalculatorGUI.answer.numerator = (arg.numerator.multiply(this.denominator)).add(this.numerator.multiply(arg.denominator));
        BigFractionCalculatorGUI.answer.denominator = arg.denominator.multiply(this.denominator);
        
        return reduction(
    			new BigFraction(
    					BigFractionCalculatorGUI.answer.numerator, 
    					BigFractionCalculatorGUI.answer.denominator
    			)
    		);
    }

    /**
     * Function for subtracting two fractions
     * @param arg1 Numerator and denominator of two fractions
     * @return Result of subtracting two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction subtraction(BigFraction arg) {
        BigFractionCalculatorGUI.answer.numerator = (this.numerator.multiply(arg.denominator)).subtract(arg.numerator.multiply(this.denominator));
        BigFractionCalculatorGUI.answer.denominator = this.denominator.multiply(arg.denominator);
        
        return reduction(
    			new BigFraction(
    					BigFractionCalculatorGUI.answer.numerator, 
    					BigFractionCalculatorGUI.answer.denominator
    			)
    		);
    }


    /**
     * Function for multiplication two fractions
     * @param arg Numerator and denominator of two fractions
     * @return Result of multiplication two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction multiplication(BigFraction arg) {
        BigFractionCalculatorGUI.answer.numerator = arg.numerator.multiply(this.numerator);
        BigFractionCalculatorGUI.answer.denominator = arg.denominator.multiply(this.denominator);
        
        return reduction(
    			new BigFraction(
    					BigFractionCalculatorGUI.answer.numerator, 
    					BigFractionCalculatorGUI.answer.denominator
    			)
    		);
    }

    /**
     * Function for division two fractions
     * @param arg Numerator and denominator of two fractions
     * @return Result of division two fractions (numerator and denominator) + initial values of the arg parameter
     */
    public BigFraction division(BigFraction arg) {
        BigFractionCalculatorGUI.answer.numerator = this.numerator.multiply(arg.denominator);
        BigFractionCalculatorGUI.answer.denominator = this.denominator.multiply(arg.numerator);
        
        return reduction(
        			new BigFraction(
        					BigFractionCalculatorGUI.answer.numerator, 
        					BigFractionCalculatorGUI.answer.denominator
        			)
        		);
    }

    /**
     * Function for reducing fractions
     * @param arg Numerator and denominator of two fractions
     * @return Reduced numerators and denominators of operands and result
     */
    public BigFraction reduction(BigFraction arg) {
    	BigInteger commonDiv = arg.numerator.gcd(arg.denominator);
        BigFractionCalculatorGUI.answer.numerator = arg.numerator.divide(commonDiv);
        BigFractionCalculatorGUI.answer.denominator = arg.denominator.divide(commonDiv);

        return BigFractionCalculatorGUI.answer;
    }

}
