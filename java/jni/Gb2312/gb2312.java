class gb2312 {
public static void main(String[] args)throws Exception {
        String str = "系统升级，请稍后";

        byte arr[]=str.getBytes("gb2312");

        for(int i = 0; i < arr.length; i++)
        System.out.println((arr[i]&0xFF) + "");
        System.out.println("lenth :"+arr.length);
    }
};
