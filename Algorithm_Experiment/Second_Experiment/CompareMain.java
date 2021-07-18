package Second_Experiment;

import edu.princeton.cs.algs4.StdStats;
import java.util.Random;
import java.util.Scanner;

public class CompareMain {
    private static double[][] experiments=new double[5][10];
    private static int[] RandomIndex;

    public CompareMain(int[] Randomindex){
        int index[]=new int[Randomindex.length];
        for (int i=0;i<12;i++){ //运行十次，i<10

            //传入5个算法中的参数为index数组，原始数组存放在Randomindex中，所以每次计算之间需要重新将原始数组赋给index
            System.arraycopy(Randomindex,0,index,0,Randomindex.length);
            long start_time=System.nanoTime();
            Insertion_Sort insertion_sort=new Insertion_Sort(index);
            long consumingtime=System.nanoTime()-start_time;  //计算总耗费时间，nanoTime单位为纳秒
            if(i>=2) experiments[0][i-2]=consumingtime*1.0/1000000;      //experiments数组记录5个算法各10次运行的时间

            System.arraycopy(Randomindex,0,index,0,Randomindex.length);
            start_time=System.nanoTime();
            Topdown_MergeSort topdown_mergeSort=new Topdown_MergeSort(index);
            consumingtime=System.nanoTime()-start_time;  //计算总耗费时间
            if(i>=2) experiments[1][i-2]=consumingtime*1.0/1000000;

            System.arraycopy(Randomindex,0,index,0,Randomindex.length);
            start_time=System.nanoTime();
            Bottomup_MergeSort bottomup_mergeSort=new Bottomup_MergeSort(index);
            consumingtime=System.nanoTime()-start_time;  //计算总耗费时间
            if(i>=2) experiments[2][i-2]=consumingtime*1.0/1000000;

            System.arraycopy(Randomindex,0,index,0,Randomindex.length);
            start_time=System.nanoTime();
            Random_QuickSort random_quickSort=new Random_QuickSort(index);
            consumingtime=System.nanoTime()-start_time;  //计算总耗费时间
            if(i>=2) experiments[3][i-2]=consumingtime*1.0/1000000;

            System.arraycopy(Randomindex,0,index,0,Randomindex.length);
            start_time=System.nanoTime();
            QuickSort_With_D3P quickSort_with_d3P=new QuickSort_With_D3P(index);
            consumingtime=System.nanoTime()-start_time;  //计算总耗费时间
            if(i>=2) experiments[4][i-2]=consumingtime*1.0/1000000;
        }
    }
    public static void main(String[] args){
        Random rd=new Random();       //随机
        Scanner in=new Scanner(System.in);
        System.out.print("Please enter the amount of data you want to enter: ");
        int count=in.nextInt();       //输入数据规模
        RandomIndex=new int[count];   //生成count数量的随机数，存放在数组RandomIndex中
        /*for (int i=0;i<count;i++){
            RandomIndex[i]=rd.nextInt(10000);//生成10000以内的随机数
        }*/
        /*for (int i=0;i<count;i++){                 //升序测试
            RandomIndex[i]=i;
        }*/
        for (int i=0;i<count;i++){                 //降序测试
            RandomIndex[i]=count-i;
        }
        CompareMain compareMain=new CompareMain(RandomIndex);
        for (int i=0;i<5;i++){
            System.out.println("-----------------------------------------------------------------------------------------------------------------------");
            if(i==0) System.out.println("Insertion Sort Time: ");
            else if(i==1) System.out.println("Top-down Mergesort Time: ");
            else if(i==2) System.out.println("Bottom-up Mergesort Time: ");
            else if(i==3) System.out.println("Random Quicksort Time: ");
            else if(i==4) System.out.println("Quicksort with Dijkstra 3-way Partition Time: ");
            for (int j=0;j<10;j++){
                System.out.print(experiments[i][j]+"ms\t");
            }
            System.out.println("\nThe average running time is: "+StdStats.mean(experiments[i])+"ms");
        }
    }
}
