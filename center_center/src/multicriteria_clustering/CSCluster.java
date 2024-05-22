package multicriteria_clustering;
import java.util.HashMap;
import java.util.ArrayList;

public class CSCluster {

    private ArrayList <Integer> points;
    private HashMap<CSCluster, Double> distances;

    public CSCluster(ArrayList<Integer> set){
        distances = new HashMap<CSCluster, Double>();
        points = set;
    }

    public HashMap<CSCluster, Double> getDistances() {
        return distances;
    }

    public ArrayList<Integer> getPoints() {
        return points;
    }

    void update_distances(CSCluster merged, CSCluster del){
        this.distances.replace(merged, merged.distances.get(this));
        this.distances.remove(del);
    }

    public void merge(CSCluster c){
   
        for (CSCluster key : distances.keySet()) {

            double distmax = distances.get(key);

            if (c.getDistances().get(key) != null && c.getDistances().get(key) > distances.get(key)) {
                distmax = c.getDistances().get(key);
            }
            this.distances.replace(key, distmax);
        }
        this.distances.remove(c);
        this.points.addAll(c.getPoints());

    }

    @Override
    public String toString(){
        return "{"+this.points.toString()+"}";
    }
}
