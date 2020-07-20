package schoolApplication.app.model;

import java.util.ArrayList;
import java.util.List;

/**
 * 
 * This class represents a student within the system.
 *
 */
public class Student {
	private String name;
	private String id;
	private List<Class> classes;
	
	public Student(String name, String id) {
		super();
		this.name = name;
		this.id = id;
		this.classes = new ArrayList<Class>();
	}
	
	public void addClass(Class clazz){
		this.classes.add(clazz);
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getId() {
		return id;
	}
	public List<Class> getClasses() {
		return classes;
	}
}
