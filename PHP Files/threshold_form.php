<!DOCTYPE html>
<html lang="en">    
<head>
<meta charset="UTF-8">    
<title>Threshold Form</title>
		<style>
			label
			{
				float:left;
				width:200px;
			}
		</style>
</head>

<body>
<form action="thresh_tempup.php" method="post">

    <p>
        <label>Temperature Upper Threshold</label>
        <input type="text" name="ut" id="firstName">    
    </p>

    <input type="submit" value="Submit">
</form>


<form action="thresh_templow.php" method="post">

    <p>
        <label>Temperature Lower Threshold</label>
        <input type="text" name="lt" id="firstName">    
    </p>

    <input type="submit" value="Submit">
</form>






<form action="thresh_lightup.php" method="post">

    <p>
        <label>Light Upper Threshold</label>
        <input type="text" name="mup" id="firstName">    
    </p>

    <input type="submit" value="Submit">
</form>


<form action="thresh_lightlow.php" method="post">

    <p>
        <label>Light Lower Threshold</label>
        <input type="text" name="nlow" id="firstName">    
    </p>

    <input type="submit" value="Submit">
</form>





<form action="thresh_tiltup.php" method="post">

    <p>
        <label>Tilt Upper Threshold</label>
        <input type="text" name="tup" id="firstName">    
    </p>

    <input type="submit" value="Submit">
</form>


<form action="thresh_tiltlow.php" method="post">

    <p>
        <label>Tilt Lower Threshold</label>
        <input type="text" name="ulow" id="firstName">    
    </p>

    <input type="submit" value="Submit">
</form>

</body>
</html>