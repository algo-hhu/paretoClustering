package multicriteria_clustering;
import java.util.ArrayList;

abstract class ParetoAlgorithm{
	abstract ArrayList<Result> calculate_pareto_set(DistMap map, Integer k);
}


