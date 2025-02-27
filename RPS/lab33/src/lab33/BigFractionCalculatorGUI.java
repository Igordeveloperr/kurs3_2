package lab33;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.math.BigInteger;

public class BigFractionCalculatorGUI extends JFrame {

    /**
     * Class variable for the first argument
     */
    public static BigFraction arg1 = new BigFraction(BigInteger.ONE, BigInteger.ONE);

    /**
     * Class variable for the second argument
     */
    public static BigFraction arg2 = new BigFraction(BigInteger.ONE, BigInteger.ONE);
    
    /**
     * Class variable for result of math operation
     */
    public static BigFraction answer = new BigFraction(BigInteger.ONE, BigInteger.ONE);

    byte fraction_counter = 0;
    boolean fraction_flag = false;
    boolean end = false;
    int MAX_NUM_LEN = 15;
    
    /**
     * Buttons for numbers
     */
    protected JButton zero = new JButton("0");
    protected JButton one = new JButton("1");
    protected JButton two = new JButton("2");
    protected JButton three = new JButton("3");
    protected JButton four = new JButton("4");
    protected JButton five = new JButton("5");
    protected JButton six = new JButton("6");
    protected JButton seven = new JButton("7");
    protected JButton eight = new JButton("8");
    protected JButton nine = new JButton("9");

    /**
     * buttons for common operations
     */
    protected JButton adding_button = new JButton("+");
    protected JButton subtraction_button = new JButton("-");
    protected JButton multiplication_button = new JButton("×");
    protected JButton dividing_button = new JButton("÷");
    protected JButton answer_button = new JButton("=");


    /**
     * buttons for non-standart operations
     */
    protected JButton fruction_button = new JButton("Trait");
    protected JButton delete_button = new JButton("⇐");
    protected JButton clear_button = new JButton("C");

    /**
     * fields for input and output
     */
    protected JLabel input_field = new JLabel("");
    protected JTextField text_field = new JTextField("", 1);

    /**
     * GridBagLayout
     */
    ActionListener actionListener = new TestActionListener();
    GridBagLayout gridbag = new GridBagLayout();
    GridBagConstraints c = new GridBagConstraints();
    byte operation_counter = 0; // + or - or / or *
    String label = " ";


    /**
     * Function for buttons
     *
     * @param gridwidth
     * @param weightx
     * @param gridx
     * @param gridy
     * @param button
     */
    public void make_buttons(int gridwidth, double weightx, int gridx, int gridy, JButton button) {
        c.gridwidth = gridwidth;
        c.weightx = weightx;
        c.gridx = gridx;
        c.gridy = gridy;
        gridbag.setConstraints(button, c);
        add(button);
        button.setFont(new Font("Serif", Font.BOLD, 30));
        button.addActionListener(actionListener);
    }

    /**
     * Function for action on operation button's press
     *
     * @param operation
     * @param line      string
     */
    public void operations_buttons(byte operation, String line, String arithmetic_operation) {
        if (fraction_flag == true && line.length() != 0) {
            label = label + line + arithmetic_operation;
            input_field.setText(label);
            operation_counter = operation;
            arg1.denominator = new BigInteger(text_field.getText());
            line = "";
            text_field.setText(line);
            fraction_flag = false;
        }
    }

    public void end(String line) {
        label = " ";
        input_field.setText(label);
        operation_counter = 0;
        line = "";
        text_field.setText(line);
        fraction_counter = 0;
        fraction_flag = false;
        end = false;
    }

    /**
     * Constructor for main window
     */
    /**
     * 
     */
    public BigFractionCalculatorGUI() {
        super("Fractional calculator");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(800, 800);
        this.setLocationRelativeTo(null);
        input_field.setText(label);
        setLayout(gridbag);
        c.weighty = 1.0;
        c.fill = GridBagConstraints.BOTH;

        /**
         * Input_Field
         */
        c.gridwidth = GridBagConstraints.REMAINDER;
        gridbag.setConstraints(input_field, c);
        add(input_field);
        input_field.setComponentOrientation(ComponentOrientation.RIGHT_TO_LEFT);
        input_field.setFont(new Font("Serif", Font.BOLD, 20));

        /**
         * Text_Field
         */
        gridbag.setConstraints(text_field, c);
        add(text_field);
        text_field.setComponentOrientation(ComponentOrientation.RIGHT_TO_LEFT);
        text_field.setFont(new Font("Serif", Font.BOLD, 40));

        /**
         * Service buttons
         */
        make_buttons(1, 0.2, 3, 5, delete_button);
        make_buttons(3, 1.0, 1, 6, fruction_button);
        make_buttons(1, 1.0, 3, 3, answer_button);
        make_buttons(1, 1.0, 3, 4, clear_button);
        
        /**
         * Operations buttons
         */
        make_buttons(1, 1.0, 0, 2, dividing_button);
        make_buttons(1, 1.0, 1, 2, multiplication_button);
        make_buttons(1, 1.0, 2, 2, subtraction_button);
        make_buttons(1, 1.0, 3, 2, adding_button);
        
        /**
         * Digit buttons
         */
        make_buttons(1, 1.0, 0, 3, seven);
        make_buttons(1, 1.0, 1, 3, eight);
        make_buttons(1, 1.0, 2, 3, nine);
        make_buttons(1, 1.0, 0, 4, four);
        make_buttons(1, 1.0, 1, 4, five);
        make_buttons(1, 1.0, 2, 4, six);
        make_buttons(1, 1.0, 0, 5, one);
        make_buttons(1, 1.0, 1, 5, two);
        make_buttons(1, 1.0, 2, 5, three);
        make_buttons(1, 1.0, 0, 6, zero);
    }

    public class TestActionListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            String line = text_field.getText();
            try {
                if (end == true) {
                    end(line);
                }
                if (e.getSource() == answer_button) {
                    if (line.length() != 0 && fraction_counter == 2) {
                        arg2.denominator = new BigInteger(text_field.getText());

                        switch (operation_counter) {
                            case 1:
                                answer = arg1.addition(arg2);

                                break;
                            case 2:
                                answer = arg1.subtraction(arg2);

                                break;
                            case 3:
                                answer = arg1.multiplication(arg2);

                                break;
                            case 4:
                                answer = arg1.division(arg2);

                                break;

                        }
                        label = label + line + " = " + answer.numerator + "/" + answer.denominator;
                        input_field.setText(label);
                        line = "";
                        text_field.setText(line);
                        end = true;
                    }

                } else if (e.getSource() == clear_button) {
                    end(line);

                } else if (e.getSource() == delete_button) {

                    if (line.length() != 0)
                        line = line.substring(0, line.length() - 1);
                    text_field.setText(line);

                } else if (e.getSource() == fruction_button) {
                    if (line.length() != 0 && fraction_flag == false && fraction_counter != 2) {
                        fraction_flag = true;
                        line = text_field.getText();
                        label = label + line + "/";
                        input_field.setText(label);
                        if (operation_counter == 0) {
                            arg1.numerator = new BigInteger(text_field.getText());
                            fraction_counter = 1;

                        } else {
                            arg2.numerator = new BigInteger(text_field.getText());
//                            fraction_flag = false;
                            fraction_counter = 2;

                        }

                        line = "";
                        text_field.setText(line);
                    }


                } else if (e.getSource() == adding_button) {
                    operations_buttons((byte) 1, line, " + ");

                } else if (e.getSource() == subtraction_button) {
                    operations_buttons((byte) 2, line, " - ");


                } else if (e.getSource() == multiplication_button) {
                    operations_buttons((byte) 3, line, " × ");


                } else if (e.getSource() == dividing_button) {
                    operations_buttons((byte) 4, line, " ÷ ");


                } else {
                	if(line.length() < MAX_NUM_LEN) {
                		System.out.println(fraction_flag);
                        if (e.getSource() == zero && fraction_flag != true) {
                            text_field.setText(line + "0");

                        } 
                        else if (e.getSource() == one) {

                            text_field.setText(line + "1");

                        } else if (e.getSource() == two) {
                            text_field.setText(line + "2");

                        } else if (e.getSource() == three) {

                            text_field.setText(line + "3");

                        } else if (e.getSource() == four) {

                            text_field.setText(line + "4");

                        } else if (e.getSource() == five) {

                            text_field.setText(line + "5");

                        } else if (e.getSource() == six) {

                            text_field.setText(line + "6");

                        } else if (e.getSource() == seven) {

                            text_field.setText(line + "7");

                        } else if (e.getSource() == eight) {

                            text_field.setText(line + "8");

                        } else if (e.getSource() == nine) {

                            text_field.setText(line + "9");
                        }
                	}
                }
            } catch (Exception ee) {
                end(line);
            }
        }
    }

}



