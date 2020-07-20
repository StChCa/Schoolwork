package schoolApplication.app.model;

/**
 * 
 * This class represents a class within the system.
 *
 */
public class Class {

	private String name;
	private String section;
	private String grade;
	
	public Class(String name, String section) {
		super();
		this.name = name;
		this.section = section;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getSection() {
		return section;
	}
	public void setSection(String section) {
		this.section = section;
	}
	public String getGrade() {
		return grade;
	}
	public void setGrade(String grade) {
		this.grade = grade;
	}
}
