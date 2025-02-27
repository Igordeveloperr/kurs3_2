package main;
import java.math.BigInteger;
import java.util.Scanner;

public class Main {
    /**
     *  Flag to exit the menu
     */
    public static boolean menu_element = false;

    /**
     * Flags to indicate the input of the numerator and denominator
     */
    public static boolean numerator_flag = false, denominator_flag = false;

    /**
     * Flag for dividing by 0
     */
    public static boolean flag_division = false;
	
	public static BigFraction arg1 = new BigFraction(BigInteger.valueOf(10), BigInteger.valueOf(15)); //0.6666666666
	public static BigFraction arg2 = new BigFraction(BigInteger.valueOf(1), BigInteger.valueOf(5)); //0.2
	 
	public static void main(String[] args) {
		BigFraction res;

		while (menu_element != true) {
            System.out.println("Используй 'help' для справки.");
            System.out.println("Введите номер команды:");
            Scanner in = new Scanner(System.in);
            String element = in.nextLine();

            switch (element) {

                /**
                 * List of menu functions
                 */
                case "help":
                    System.out.println("1. Сложение");
                    System.out.println("2. Вычитание");
                    System.out.println("3. Умножение");
                    System.out.println("4. Деление");
                    System.out.println("5. Выход");
                    break;

                /**
                 * @see BigFraction#Addition(BigFraction)
                 */
                case "1":
                    menu_minimization();
                    res = arg1.Addition(arg2);
                    System.out.print(arg1 + " + " + arg2 + " = ");
                    System.out.println(res);
                    break;

                /**
                 * @see BigFraction#Subtraction(BigFraction)
                 */
                case "2":
                    menu_minimization();
                    res = arg1.Subtraction(arg2);
                    System.out.print(arg1 + " - " + arg2 + " = ");
                    System.out.println(res);
                    break;

                /**
                 * @see BigFraction#Multiplication(BigFraction)
                 */
                case "3":
                    menu_minimization();
                    res = arg1.Multiplication(arg2);
                    System.out.print(arg1 + " * " + arg2 + " = ");
                    System.out.println(res);
                    break;

                /**
                 * @see BigFraction#Division(BigFraction)
                 */
                case "4":
                    flag_division = true;
                    menu_minimization();
                    res = arg1.Division(arg2);
                    System.out.print(arg1 + " div " + arg2 + " = ");
                    System.out.println(res);
                    flag_division = false;
                    break;

                /**
                 * Exit from the program
                 */
                case "5":
                    menu_element = true;
                    break;
            }

        }

	}
	
    /**
     * Input function for code minimization
     */
    public static void menu_minimization() {
        numerator_flag = false;
        denominator_flag = false;
        System.out.println("Введите 1 число");
        InputFraction(1);
        numerator_flag = false;
        denominator_flag = false;
        System.out.println("Введите 2 число");
        InputFraction(2);
    }
    
    /**
     * Function for numerator and denominator input
     * @param number Operand number
     * @throws IndexOutOfBoundsException If one of the arguments went beyond 1000000
     * @throws ArithmeticException If any of the denominators is 0 or number is less than 0
     * @throws Exception If input number is not of the int type and all other situations
     */
    public static void InputFraction(int number) {
        try {
        	
        	InputNumerator(number);
        	InputDenominator(number);
            
        } catch (IndexOutOfBoundsException exp) {
            System.out.println(exp);
            InputFraction(number);
        } catch (ArithmeticException exp) {
            System.out.println(exp);
            InputFraction(number);
        } catch (Exception exp) {
            System.out.println("Вы ввели не число!");
            InputFraction(number);
        }
    }
    
    public static void InputNumerator(int number) {
        while (numerator_flag == false) {
            System.out.print("Числитель: ");
            Scanner first = new Scanner(System.in);

            if (number == 1) {
                arg1.SetNumerator(first.nextBigInteger());
            } else arg2.SetNumerator(first.nextBigInteger());


            int check1 = arg1.GetNumerator().compareTo(BigInteger.valueOf(1000000));
            int check2 = arg2.GetNumerator().compareTo(BigInteger.valueOf(1000000));
            int check3 = arg1.GetNumerator().compareTo(BigInteger.valueOf(-1000000));
            int check4 = arg2.GetNumerator().compareTo(BigInteger.valueOf(-1000000));
//            int check5 = arg1.GetNumerator().compareTo(BigInteger.valueOf(0));
//            int check6 = arg2.GetNumerator().compareTo(BigInteger.valueOf(0));

            if (check1 == 1 || check2 == 1) {
                throw
                    new IndexOutOfBoundsException("Очень большой числитель!");
            }
            else if (check3 == (-1) || check4 == (-1)) {
                throw
                    new IndexOutOfBoundsException("Очень маленький числитель!");
            }
//            else if (check5 <= 0 || check6 <= 0) 
//            	throw
//                    new ArithmeticException("Entering negative numbers is prohibited");
            else
                numerator_flag = true;
        }
    }
    
    
    public static void InputDenominator(int number) {
    	while (denominator_flag == false) {
            System.out.print("Знаменатель: ");
            Scanner second = new Scanner(System.in);

            if (number == 1) {
                arg1.SetDenominator(second.nextBigInteger());
            } else arg2.SetDenominator(second.nextBigInteger());


            int check1 = arg1.GetDenominator().compareTo(BigInteger.valueOf(1000000));
            int check2 = arg2.GetDenominator().compareTo(BigInteger.valueOf(1000000));
            int check3 = arg1.GetDenominator().compareTo(BigInteger.valueOf(-1000000));
            int check4 = arg2.GetDenominator().compareTo(BigInteger.valueOf(-1000000));
            int check5 = arg1.GetDenominator().compareTo(BigInteger.valueOf(0));
            int check6 = arg2.GetDenominator().compareTo(BigInteger.valueOf(0));

            if (check1 == 1 || check2 == 1) {
                throw
                        new IndexOutOfBoundsException("Очень большой знаменатель!");
            }
            else if (check3 == (-1) || check4 == (-1)){
                throw
                    new IndexOutOfBoundsException("Очень маленький знаменатель!");
            }
            else if (check5 == 0 || check6 == 0) 
            	throw
                    new ArithmeticException("Знаменатель не может быть 0!");
            else if (number == 2 && flag_division == true && check5 == 0) 
            	throw
                    new ArithmeticException("Знаменатель не может быть 0!");
            else if (check5 <= 0 || check6 <= 0) 
            	throw
                    new ArithmeticException("Ввод отрицательного числа запрещен!");
            else
                denominator_flag = true;
        }
    }

}
