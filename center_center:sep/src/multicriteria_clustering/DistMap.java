package multicriteria_clustering;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Arrays;


public class DistMap {
	
	private ArrayList<ArrayList<Double>> distances1;
	private ArrayList<ArrayList<Double>> distances2;



	private ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> sorted1;
	private ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> sorted2;
	public DistMap(ArrayList<ArrayList<Double>> first_distance, ArrayList<ArrayList<Double>> second_distance) {
		distances1 = first_distance;
		distances2 = second_distance;

		this.sorted1 = new ArrayList<>();
		this.sorted2 = new ArrayList<>();

		this.fillSortedLists();

		//printSorted1();
	}
	
	public int size() {
		return distances1.size();
	}
	
	public Double dist1(int i, int j) {
		return distances1.get(i).get(j);
	}
	
	public Double dist2(int i, int j) {
		return distances2.get(i).get(j);
	}

	public ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> getSorted1() {
		return sorted1;
	}

	public ArrayList<Pair<Double, ArrayList<Pair<Integer, Integer>>>> getSorted2() {
		return sorted2;
	}

	private void fillSortedLists(){
		HashMap<Double, ArrayList<Pair<Integer, Integer>>> map1 = new HashMap<Double, ArrayList<Pair<Integer, Integer>>>();
		HashMap<Double, ArrayList<Pair<Integer, Integer>>> map2 = new HashMap<Double, ArrayList<Pair<Integer, Integer>>>();
		for(int i = 1; i < this.size(); i ++){
			for( int j = 0; j < i; j++){
				double dist1 = this.dist1(i, j);
				double dist2 = this.dist2(i, j);
				if(!map1.containsKey(dist1)) {
					map1.put(dist1, new ArrayList<Pair<Integer, Integer>>());
				}
				if(!map2.containsKey(dist2)) {
					map2.put(dist2, new ArrayList<Pair<Integer, Integer>>());
				}
				map1.get(dist1).add(new Pair(i, j));
				map2.get(dist2).add(new Pair(i, j));
			}
		}
		for (double d : map1.keySet()) {
			this.sorted1.add(new Pair(d, map1.get(d)));
		}
		for (double d : map2.keySet()) {
			this.sorted2.add(new Pair(d, map2.get(d)));
		}
		Collections.sort(sorted1);
		Collections.sort(sorted2);
	}

	public void printSorted1(){
		System.out.println(Arrays.toString(sorted1.toArray()));
	}

	public void printSorted2(){
		System.out.println(Arrays.toString(sorted2.toArray()));
	}
}
