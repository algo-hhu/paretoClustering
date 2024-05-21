package multicriteria_clustering;

import java.util.ArrayList;
import java.util.HashMap;


public class CenterSeparation extends ParetoAlgorithm{

    private ArrayList<CSCluster> cluster;
    private HashMap<Integer, CSCluster> point_location;

    public CenterSeparation(){
        this.cluster = new ArrayList<CSCluster>();
        this.point_location = new HashMap<Integer, CSCluster>();
    }

    @Override
    public ArrayList<Result> calculate_pareto_set(DistMap map, Integer k){

        createSingleClusters(map);


        ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> sorted1 = map.getSorted1();
        ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> sorted2 = map.getSorted2();

        int i = 0;
        int j =-1;

        double r;
        double d = 0;

        boolean flag = false;

        ArrayList<Result> solutions = new ArrayList<Result>();
        Result currentbest = null;

        while(j < sorted2.size() && i < sorted1.size()){

            r = sorted1.get(i).getElement0();
            if(j>=0) {
                d = sorted2.get(j).getElement0();



                ArrayList<Pair<Integer, Integer>> edges = sorted2.get(j).getElement1();

                for (int l = 0; l < edges.size(); l++) {
                    ;
                    int node1 = edges.get(l).getElement0();
                    int node2 = edges.get(l).getElement1();

                    CSCluster cluster1 = point_location.get(node1);
                    CSCluster cluster2 = point_location.get(node2);


                    if (cluster1 != cluster2) {

                        merge(cluster1, cluster2);

                    }
                }
            }

            Result solution = computeSolution(k, map.size(), r);



            if(solution == null){
                if(flag){

                    solutions.add(currentbest);
                }
                flag = false;
                currentbest = null;
                i++;

            }
            else {

                flag = true;
                currentbest = solution;

                j++;

                if(j == sorted2.size()||cluster.size()==k){
                    solutions.add(currentbest);
                    return solutions;
                }
            }
        }
        return solutions;
    }

    public Result computeSolution(int k, int n, double r){
        for(CSCluster c: cluster) {
            if (!(c.getDistances().get(c) <= r)) {
                return null;
            }
        }
        if(cluster.size() <= k) {
           return create_solution_from_clusters();
        }

        else{
            Result result = new Result();

            ArrayList<Integer> points = new ArrayList<>();
            HashMap<Integer, Integer> assignment = new HashMap<Integer, Integer>();
            int num_remaining_clusters = cluster.size();

            for(int i = 0; i< n; i++){
                points.add(i);
            }

            if (result.centers.size() > k) {
                return null;
            }

            while((!points.isEmpty())) {


                int center = points.getFirst();
                result.centers.add(center);

                CSCluster loc = point_location.get(center);

                for(int p: loc.getPoints()){
                    assignment.put(p, center);
                    points.remove(Integer.valueOf(p));

                }
                num_remaining_clusters--;

                for (CSCluster cl : cluster) {
                    if(result.centers.size() + num_remaining_clusters > k) {
                        if (loc.getDistances().get(cl) <= r && points.contains(cl.getPoints().getFirst())) {

                            num_remaining_clusters--;

                            for (int p : cl.getPoints()) {

                                assignment.put(p, center);
                                points.remove(Integer.valueOf(p));
                            }
                        }
                    }
                    else {

                        break;
                    }
                }
            }
            if(result.centers.size()>k){
                return null;
            }
            else {
                for (int i = 0; i < n; i++) {
                    result.assignment.add(assignment.get(i));
                }
                return result;
            }
        }
    }

    public void merge(CSCluster c1, CSCluster c2){

        c1.merge(c2);
        ArrayList<Integer> pointstomove = c2.getPoints();
        for (Integer integer : pointstomove) {
            point_location.replace(integer, c1);
        }
        for (CSCluster csCluster : cluster) {
            csCluster.update_distances(c1, c2);
        }
        cluster.remove(c2);
    }

    public void createSingleClusters(DistMap map){
        int n = map.size();
        for(int i = 0; i<n; i++){
            this.point_location.put(i, new CSCluster(new ArrayList<Integer>()));
            this.point_location.get(i).getPoints().add(i);
            this.cluster.add(this.point_location.get(i));
        }
        for(int i = 0; i<n; i++){
            for(int j = 0; j<n; j++){
                this.point_location.get(i).getDistances().put(this.point_location.get(j), map.dist1(i, j));
            }
        }
    }

    public Result create_solution_from_clusters(){
        Result result = new Result();
        for(CSCluster c: cluster){
            int center = c.getPoints().getFirst();
            result.centers.add(center);
        }
        for(int i = 0; i < point_location.size(); i++){
            result.assignment.add(point_location.get(i).getPoints().getFirst());
        }
        return result;

    }
}
