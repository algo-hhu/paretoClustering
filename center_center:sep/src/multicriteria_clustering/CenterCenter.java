package multicriteria_clustering;
import java.util.ArrayList;

public class  CenterCenter extends ParetoAlgorithm{
	ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> distances_1;
	ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> distances_2;
	CCGraph graph;
	ArrayList<Result> pareto_set;

	public ArrayList<Result> calculate_pareto_set(DistMap map, Integer k) {
		distances_1 = map.getSorted1();
		distances_2 = map.getSorted2();
		graph = new CCGraph(map.size());
		pareto_set = new ArrayList<Result>();
		int j = 0;
		double outer_distance = distances_1.get(distances_1.size() - 1).getElement0();
		double inner_distance = distances_2.get(0).getElement0();
		Boolean dominates = false;
		for(int i = distances_1.size()- 1; i >= 0; i--){
			outer_distance = distances_1.get(i).getElement0();
			dominates = true;
			while(graph.indep_size() > k && j < distances_2.size()){
				dominates = false;
				for(Pair<Integer, Integer> edge: distances_2.get(j).getElement1()){
					if(map.dist1(edge.getElement0(), edge.getElement1()) <= outer_distance){
						graph.add_edge(edge.getElement0(), edge.getElement1());
					}
				}
				inner_distance = distances_2.get(j).getElement0();
				j++;
			}
			if(graph.indep_size() <= k){
				if(dominates == true){
					pareto_set.set(pareto_set.size() - 1, graph.return_result());
				}
				else{
					pareto_set.add(graph.return_result());
				}
				pareto_set.get(pareto_set.size() - 1).cost1 = outer_distance;
				pareto_set.get(pareto_set.size() - 1).cost2 = inner_distance;
			}
			for(Pair<Integer, Integer> edge: distances_1.get(i).getElement1()){
				if(map.dist2(edge.getElement0(), edge.getElement1()) <= inner_distance){
					graph.remove_edge(edge.getElement0(), edge.getElement1());
				}
			}
		}
		return pareto_set;
	}

}
