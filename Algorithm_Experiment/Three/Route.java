package Three;

import edu.princeton.cs.algs4.*;
import java.text.DecimalFormat;
import java.util.Scanner;

class Dijkstra_change {
    private double[] distTo;
    private Edge[] edgeTo;
    private IndexMultiwayMinPQ<Double> pq;
    public Dijkstra_change(EdgeWeightedGraph G, int s, int d, double[][] Nodes) {
        for (Edge e : G.edges()) {
            if (e.weight() < 0)
                throw new IllegalArgumentException("edge " + e + " has negative weight");
        }
        distTo = new double[G.V()];
        edgeTo = new Edge[G.V()];
        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;
        pq = new IndexMultiwayMinPQ<Double>(G.V(),2);
        pq.insert(s, distTo[s]);
        while (!pq.isEmpty()) {
            int v = pq.delMin();
            /*if (v == d)                // 优化1，当目标节点已发现，停止运行
                return;*/
            for (Edge e : G.adj(v))    //从距离最近节点的邻居节点开始遍历
                relax(e, v, Nodes, d); //更新dist数组的值
        }
    }

    private void relax(Edge e, int v, double[][] Nodes, int d) {
        int w = e.other(v);           // distTo[W] = distTo[V] - V2D + U->V + W2D；A*算法距离公式；
        double newDist = distTo[v] + e.weight() + Route.Distance(w,d) - Route.Distance(v,d);
        /*if (distTo[w]- 0.000001 > newDist) {
            distTo[w] = newDist;     //更新dist数组
            edgeTo[w] = e;           //记录最短距离选择的边
            if (pq.contains(w))      //重新插入到队列之中
                pq.decreaseKey(w, distTo[w]);
            else
                pq.insert(w, distTo[w]);
        }*/
        if (distTo[w] > distTo[v] + e.weight()) {
            distTo[w] = distTo[v] + e.weight();
            edgeTo[w] = e;
            if (pq.contains(w)) pq.decreaseKey(w, distTo[w]);
            else                pq.insert(w, distTo[w]);
        }
    }
    public boolean hasPathTo(int v) {   //判断目标节点是否可达
        return distTo[v] < Double.POSITIVE_INFINITY;
    }
    public Iterable<Edge> pathTo(int v) {
        if (!hasPathTo(v)) return null;
        Stack<Edge> path = new Stack<Edge>();
        int x = v,j=0;
        String result[] = new String[10000];
        for(Edge e = edgeTo[v]; e != null; e = edgeTo[x]) {
            path.push(e);
            DecimalFormat df = new DecimalFormat("#.00");
            String str = df.format(e.weight());
            result[j++]="<"+e.other(x) + "-" + x +"> --- " + str;
            x = e.other(x);
        }
        for (int i=0;i<j;i++){
            StdOut.println(result[j-i-1]);
        }
        return path;
    }
}

public class Route {
    private static EdgeWeightedGraph G;  //图数据
    private static double[][] Nodes;     //记录节点坐标
    public static void Create_Graph(In in) {
        int V = in.readInt();
        int E = in.readInt();
        G = new EdgeWeightedGraph(V);
        Nodes = new double[V][2];
        int v;
        for (int i = 0; i < V; i++) {
            v = in.readInt();
            Nodes[v][0] = in.readDouble();
            Nodes[v][1] = in.readDouble();
        }
        int v1, v2;
        for (int i = 0; i < E; i++) {    //读取边数据，并插入到图中
            v1 = in.readInt();
            v2 = in.readInt();
            G.addEdge(new Edge(v1, v2, Distance(v1,v2)));
        }
    }
    public static double Distance(int v1, int v2) {//计算两个节点之间的距离
        return Math.sqrt(Math.pow((Nodes[v1][0]-Nodes[v2][0]), 2) + Math.pow((Nodes[v1][1]-Nodes[v2][1]), 2));
    }
    public static void main(String[] args) {
        In in = new In("G:\\web_file\\SF_Travis\\src\\Three\\usa.txt");
        Scanner input =new Scanner(System.in);
        StdOut.print("Please enter a start node: ");
        int start =input.nextInt();
        StdOut.print("Please enter an end node: ");
        int end =input.nextInt();
        Create_Graph(in);

        long startTime = System.currentTimeMillis();
        Dijkstra_change DC= new Dijkstra_change(G, start, end, Nodes);
        long endTime = System.currentTimeMillis();

        if (DC.hasPathTo(end)) {
            double count = 0;
            for (Edge e : DC.pathTo(end)) {
                count += e.weight();
            }
            StdOut.println("Length of path: " + count);
        } else {
            StdOut.println("Path doesn't exist!");
        }
        StdOut.println("-------The program running time is " + (double)(endTime - startTime) + "ms -------");
    }
}
