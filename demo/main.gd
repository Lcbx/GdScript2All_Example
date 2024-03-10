extends Node3D


func _ready():
	
	var test : String = gd2AllTest.hello_world();
	print(test)
	
	await(get_tree().create_timer(5).timeout)
	get_tree().quit()


