package multicriteria_clustering;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;  
import java.io.IOException;  
import java.io.*;  
import java.util.Scanner;  



public class main {
	public static void main(String[] args) throws Exception  
	{
		String distance_file1 = args[0];
		String distance_file2 = args[1];
		int k = Integer.parseInt(args[2]);
		String solutions_prefix = args[3];
		String pareto_file = args[4];

		ArrayList<ArrayList<Double>> distances = getDistances(distance_file1);
		ArrayList<ArrayList<Double>> distances2 = getDistances(distance_file2);

		DistMap map = new DistMap(distances, distances2);


		//CenterSeparation algorithmCS = new CenterSeparation();
		CenterCenter algorithmCC = new CenterCenter();


		ArrayList<Result> paretoSet = algorithmCC.calculate_pareto_set(map, k);

		int total_sol_number = paretoSet.size();
		System.out.println(total_sol_number);

		for(int i = 1; i <= total_sol_number; i++) {

			String filename = solutions_prefix + k + "Sol" + i + "of" + total_sol_number + ".json";

			paretoSet.get(i-1).toJsonFile(filename, false);
		}

		paretofront_to_file(pareto_file, paretoSet);

	}


	public static ArrayList<ArrayList<Double>> getDistances(String file) throws Exception  {
		String line = "";
		String splitBy = ";";
		ArrayList<ArrayList<Double>> distances=new ArrayList<>();

		BufferedReader br = new BufferedReader(new FileReader(file));

		while ((line = br.readLine()) != null)   //returns a Boolean value
		{
			String[] station = line.split(splitBy);    // use comma as separator
			ArrayList<Double> station_double = new ArrayList<>();
			for(String d : station) {
				station_double.add(Double.valueOf(d));
			}
			distances.add(station_double);
		}
		br.close();
		return distances;

	}


	//creates a csv containing the paretofront
	public static void paretofront_to_file (String filename, ArrayList<Result> paretoSet) throws IOException{
		int total_sol_number = paretoSet.size();


		double [][] val_array = new double[total_sol_number][2];

		for(int i = 1; i <= total_sol_number; i++) {

			paretoSet.get(i - 1).toJsonFile(filename, false);

			val_array[i - 1][0] = paretoSet.get(i - 1).cost1;
			val_array[i - 1][1] = paretoSet.get(i - 1).cost2;
		}

		BufferedWriter outputWriter = null;
		outputWriter = new BufferedWriter(new FileWriter(filename));

		for (int i = 0; i < val_array.length; i++) {

			outputWriter.write("Threshold "+val_array[i][0]);
			outputWriter.newLine();
			outputWriter.write("Cost "+val_array[i][1]);
			outputWriter.newLine();
		}
		outputWriter.flush();
		outputWriter.close();
	}
}
