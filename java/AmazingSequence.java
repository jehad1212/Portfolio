
import java.util.*;
import java.util.Scanner;

public class AmazingSequence {
    public static void main(String[] args) {
        int[][] input = get_input();
        int flag = input[0][0];
        int N = input[1][0];
        int[] seq = input[2];

        // check sequence for 0 first number and increasing order
        boolean increase = increasing(seq);

        int[] differences = get_differences(seq);

        int dups = countDuplicates(differences);

  
        if (flag == 1){
            if (dups == 0 && increase){
                System.out.println("It is an Amazing Sequence");
            }
            else{
                System.out.println("It is an ordinary sequence");
            }
        }
        else if (flag == -1) {
            System.out.println("Error in input");
        }
        else{
            System.out.println("Invalid input");
        }

    }

   
    public static int[][] get_input() {

        // Variable Declarations
        Scanner scan = new Scanner(System.in);
        int N = 0;
        int flag = 1;

        // Take size of array
        if (scan.hasNextInt()) {
            N = scan.nextInt();
        } else {
            // bad input
            flag = -1;
            int[] fl = { flag };
            int[] num = { 0 };
            int[][] ret = { fl, num, num };
            return ret;
        }

        // check for more input on N line
        // if (scan.hasNext()) {
        //     flag = 0;
        // }

        // check bad input & force N > 0
        if (!(N > 0)) {
            flag = -1;
            N = 1;
            int[] fl = { flag };
            int[] num = { 0 };
            int[][] ret = { fl, num, num };
            return ret;
            
        }

        // define array
        int[] seq = new int[N];

        String s = scan.nextLine();

        String input = scan.nextLine();
        String[] parts = input.split("\\s+");

        // Convert to integer array and check for bad input
        if (parts.length != N) {
            flag = 0;
        } else {
            try {
                for (int i = 0; i < parts.length; i++) {
                    seq[i] = Integer.parseInt(parts[i]);
                    // check for negatives
                    if (Integer.parseInt(parts[i]) < 0) {
                        flag = 0;
                    }
                }
            } catch (Exception e) {
                flag = 0;
            }
        }
        // setting up return value
        
  

        int[] fl = { flag };
        int[] num = { N };
        int[][] ret = { fl, num, seq };
        return ret;
    }

    public static int[] get_differences(int[] seq){

        int size = (seq.length * seq.length - seq.length) / 2;
        int[] ret = new int[size];
        int i = 0;
        for (int x = 0; x < seq.length; x++){
            
            for (int y = x+1; y< seq.length; y++){
                ret[i] = Math.abs(seq[x] - seq[y]);
                i++;
            }
        }
        return ret;
    }

    public static int countDuplicates(int [] arr) {

        int dups = 0;
     


        for (int i = 0; i<arr.length; i++){
            
            int count = 0;

            for (int y = 0; y < i; y++ ){
                if (arr[i] == arr[y]){
                    count ++;
                }
            }

            if (count == 1){
                dups ++;
            }

        
        }
        return dups;
}

    public static boolean increasing(int[] seq) {

        if (seq.length < 1){return false;}

        boolean flag = true;

        if (seq[0] != 0){
            flag = false;
        }

        for(int i = 0; i < seq.length; i++){
            for( int y = i; y < seq.length; y++ ){
                if(seq[i] > seq[y]){
                    flag = false;
                }
            }
        }
        return flag;

}
}
