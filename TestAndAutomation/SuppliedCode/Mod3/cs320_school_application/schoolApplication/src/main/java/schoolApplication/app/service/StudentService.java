package schoolApplication.app.service;

import java.util.ArrayList;
import java.util.List;
import schoolApplication.app.model.Student;
import schoolApplication.app.model.Class;

public class StudentService {
	
	private static StudentService reference = new StudentService();
	private List<Student> students;

	public static StudentService getReference() {
		return reference;
	}

	StudentService() {
		this.students = new ArrayList<Student>();
	}
	
	public Student createStudent(String name, String id){
		Student student = new Student(name, id);
		students.add(student);
		return student;
	}
	
	public Student assignClass(String studentId, Class clazz){
		Student student = getStudent(studentId);
		if(student != null){
			student.addClass(clazz);
		}
		return student;
	}
	
	public Student assignClassGradeToStudent(String studentId, String className, String classSection, String grade){
		Student student = getStudent(studentId);
		if(student != null){
			Class theClazz = student.getClasses().stream().filter(clazz -> clazz.getName().equals(className) && clazz.getSection().equals(classSection)).findFirst().get();
			if(theClazz != null){
				theClazz.setGrade(grade);
			}
		}
		return student;
	}
	
	public Student getStudent(String studentId){
		return students.stream().filter(student -> student.getId().equals(studentId)).findFirst().get();
	}


	public Student updateGrade(String studentId, String className, String classSection, String newGrade){
		return assignClassGradeToStudent(studentId, className, classSection, newGrade);
	}
}
