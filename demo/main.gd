extends Node3D

func _ready():
	compileShaderTest()


func compileShaderTest():
	
	var shaderCode : String = gd2AllTest.hello_world();
	print(shaderCode)
	
	await(get_tree().create_timer(5).timeout)
	get_tree().quit()


