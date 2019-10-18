public class Dog {
        String name;

        void bark() {
                System.out.print(name + " is barking\n");
        }
};

class DogTest {

        public static void main(String arg[]) {

                Dog myDog = new Dog();

                myDog.name = "haha";

                myDog.bark();

                System.out.print(arg[1] + arg[0] + "\n");

                try {
                        readThefile("test.txt");
                }
                catch (FileNotFoundException ex) {
                        System.out.print("file not found\n");
                };
        };
};
