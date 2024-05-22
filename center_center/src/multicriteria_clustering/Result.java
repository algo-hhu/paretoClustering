package multicriteria_clustering;
import java.io.*;
import java.lang.reflect.Type;
import java.util.ArrayList;

import com.google.gson.reflect.TypeToken;
import org.json.JSONObject;
import java.util.HashMap;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;




public class Result {
	
	public Result() {
		centers = new ArrayList<Integer>();
		assignment = new ArrayList<Integer>();
	}
	
	public double cost1;
	public double cost2;
	public ArrayList<Integer> centers;
	public ArrayList<Integer> assignment;


	//for plotting
	public void toJsonFile(String output_file, boolean sealevel_data) throws IOException {
		HashMap<Integer, Integer> index_to_id = null;
		if(sealevel_data) {

			try {
				String jsonFilePath = "distances/indextoid.json";

				FileReader reader = new FileReader(jsonFilePath);

				Type type = new TypeToken<HashMap<Integer, Integer>>() {
				}.getType();
				index_to_id = new Gson().fromJson(reader, type);

				reader.close();

				//System.out.println("HashMap: " + index_to_id.get(3));

			} catch (IOException e) {
				e.printStackTrace();
			}
		}



		HashMap<Integer, ArrayList<Integer>> map = new HashMap<>();
		for (Integer c : centers){
			if(sealevel_data) {
				c = index_to_id.get(c);
			}
			map.put(c, new ArrayList<Integer>());
		}
		for (int i = 0; i < assignment.size(); i++){
			int p = i;
			int c = assignment.get(p);
			if(sealevel_data) {
				p = index_to_id.get(p);
				c = index_to_id.get(assignment.get(i));
			}
			map.get(c).add(p);
		}

		JSONObject json = new JSONObject(map);

		try {

			FileWriter file = new FileWriter(output_file);
			file.write(json.toString());
			file.close();

		} catch (IOException e) {

			e.printStackTrace();
		}
	}


}
