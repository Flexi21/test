public class Hello {

    static {
        System.loadLibrary("hello");
    }

    public static native int hello();
 
    public static void main(String arg[]){
 
        hello();

    }
   
}
