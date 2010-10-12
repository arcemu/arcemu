<?php
include("include/header.inc.php");
?>
Aura 107 and 108 uses spell groups to influence their values. <br>
Since 1.12 spell groups are no longer available to be used so we need them in order to have functional spells<br>
Showing info about spell groups:<br>
<b>=========================================================================================================================================</b><br>
<b>list of spells for aura 107 </b><br>
<?php	$a_info[107] = list_aura_info(107); ?>
<b>=========================================================================================================================================</b><br>
<b>list of spells for aura 108</b><br>
<?php 	$a_info[108] = list_aura_info(108); ?>
<b>=========================================================================================================================================</b><br>
<?php
//count those that contain at least 1 spell name
$aura=$a_info[107];
for($i=0;$i<$aura['lines'];$i++)
{
	$spelldesc=$aura[$i]['desc'];
	if(!$spelldesc)
		continue;
	$descparts=explode("\$",$spelldesc);
	$get_query="select distinct(name) from dbc_spell where 1=1 order by name desc";
	$get_res=mysql_query($get_query,$dbi) or die(" Q2007082018031 ".mysql_error($dbi)."<br>".$get_query);
	$aura[$i][0]['spells']=0;
	$aura[$i][1]['spells']=0;
	$aura[$i][2]['spells']=0;
	while(list($cur_name)=mysql_fetch_row($get_res))
		for($k=0;$k<3;$k++)
			if(strpos($descparts[$k],$cur_name))
			{
				$aura[$i][$k][$aura[$i][$k]['spells']] = $cur_name;
				$aura[$i][$k]['spells']++;
			}
}
//now print a table for those that we found spell names
?>
listing spells for which we might generate spellgroups <br>
	<table border='1'>
		<tr>
			<td>number</td>
			<td>name</td>
			<td>desc</td>
			<td>spellnames</td>
		</tr>
<?php
$line_counter=0;
for($i=0;$i<$aura['lines'];$i++)
{
	$show=0;
	for($k=0;$k<3;$k++)
		if($aura[$i][$k]['spells'])
			$show=1;
	if($show)
		{
			$line_counter++;
			$spellnames="";
			for($k=0;$k<3;$k++)
				if($aura[$i][$k]['spells'])
				{
					$spellnames .= ($k+1).") ";
					for($j=0;$j<$aura[$i][$k]['spells'];$j++)
						$spellnames .= $aura[$i][$k][$j]." # ";
					$spellnames.="<br>";
				}
			?>
			<tr>
				<td><?=$line_counter?></td>
				<td><?=$aura[$i]['name']?></td>
				<td><?=$aura[$i]['desc']?></td>
				<td><?=$spellnames?></td>
			</tr>
			<?php
		}
}
//now print a table for those that we found spell names
?>
</table>
<b>=========================================================================================================================================</b><br>
listing spells for which we could not guess any spell group spellgroups <br>
	<table border='1'>
		<tr>
			<td>number</td>
			<td>name</td>
			<td>desc</td>
			<td>spellnames</td>
		</tr>
<?php
$line_counter=0;
$spellnames="";
for($i=0;$i<$aura['lines'];$i++)
{
	$show=0;
	for($k=0;$k<3;$k++)
		if($aura[$i][$k]['spells'])
			$show=1;
	if($show==0)
		{
			$line_counter++;
			?>
			<tr>
				<td><?=$line_counter?></td>
				<td><?=$aura[$i]['name']?></td>
				<td><?=$aura[$i]['desc']?></td>
				<td><?=$spellnames?></td>
			</tr>
			<?php
		}
}
?>
</table>
<b>=========================================================================================================================================</b><br>
<b>=========================================================================================================================================</b><br>
<?php
//count those that contain at least 1 spell name
$aura=$a_info[108];
for($i=0;$i<$aura['lines'];$i++)
{
	$spelldesc=$aura[$i]['desc'];
	if(!$spelldesc)
		continue;
	$descparts=explode("\$",$spelldesc);
	$get_query="select distinct(name) from dbc_spell where 1=1 order by name desc";
	$get_res=mysql_query($get_query,$dbi) or die(" Q2007082018031 ".mysql_error($dbi)."<br>".$get_query);
	$aura[$i][0]['spells']=0;
	$aura[$i][1]['spells']=0;
	$aura[$i][2]['spells']=0;
	while(list($cur_name)=mysql_fetch_row($get_res))
		for($k=0;$k<3;$k++)
			if(strpos($descparts[$k],$cur_name))
			{
				$aura[$i][$k][$aura[$i][$k]['spells']] = $cur_name;
				$aura[$i][$k]['spells']++;
			}
}
//now print a table for those that we found spell names
?>
listing spells for which we might generate spellgroups <br>
	<table border='1'>
		<tr>
			<td>number</td>
			<td>name</td>
			<td>desc</td>
			<td>spellnames</td>
		</tr>
<?php
$line_counter=0;
for($i=0;$i<$aura['lines'];$i++)
{
	$show=0;
	for($k=0;$k<3;$k++)
		if($aura[$i][$k]['spells'])
			$show=1;
	if($show)
		{
			$line_counter++;
			$spellnames="";
			for($k=0;$k<3;$k++)
				if($aura[$i][$k]['spells'])
				{
					$spellnames .= ($k+1).") ";
					for($j=0;$j<$aura[$i][$k]['spells'];$j++)
						$spellnames .= $aura[$i][$k][$j]." # ";
					$spellnames.="<br>";
				}
			?>
			<tr>
				<td><?=$line_counter?></td>
				<td><?=$aura[$i]['name']?></td>
				<td><?=$aura[$i]['desc']?></td>
				<td><?=$spellnames?></td>
			</tr>
			<?php
		}
}
//now print a table for those that we found spell names
?>
</table>
<b>=========================================================================================================================================</b><br>
listing spells for which we could not guess any spell group spellgroups <br>
	<table border='1'>
		<tr>
			<td>number</td>
			<td>name</td>
			<td>desc</td>
			<td>spellnames</td>
		</tr>
<?php
$line_counter=0;
$spellnames="";
for($i=0;$i<$aura['lines'];$i++)
{
	$show=0;
	for($k=0;$k<3;$k++)
		if($aura[$i][$k]['spells'])
			$show=1;
	if($show==0)
		{
			$line_counter++;
			?>
			<tr>
				<td><?=$line_counter?></td>
				<td><?=$aura[$i]['name']?></td>
				<td><?=$aura[$i]['desc']?></td>
				<td><?=$spellnames?></td>
			</tr>
			<?php
		}
}
?>
	</table>
<b>=========================================================================================================================================</b><br>


<?php
function list_aura_info($aura_name)
{
	global $dbi;
	$ret="";
	$get_query="select distinct(name),description,EffectSpellGroupRelation_1,EffectSpellGroupRelation_2,EffectSpellGroupRelation_3,EffectApplyAuraName_1,EffectApplyAuraName_2,EffectApplyAuraName_3 from dbc_spell where (EffectApplyAuraName_1=$aura_name or EffectApplyAuraName_2=$aura_name or EffectApplyAuraName_3=$aura_name) order by name desc";
	$get_res=mysql_query($get_query,$dbi) or die(" Q2007082018031 ".mysql_error($dbi)."<br>".$get_query);
	$might_work=0;
	$line_counter=0; 
	$not_used=0;
	?>
	<table border='1'>
		<tr>
			<td>number</td>
			<td>name</td>
			<td>desc</td>
			<td>works</td>
		</tr>
<?php
	while(list($name,$desc,$gr1,$gr2,$gr3,$a1,$a2,$a3)=mysql_fetch_row($get_res))
	{
		$works=0;
		if(($a1==$aura_name && $gr1!=0) || ($a2==$aura_name && $gr2!=0) || ($a3==$aura_name && $gr3!=0))
			$works=1;
		$might_work += $works;
		if(strlen($desc)<5)
			$not_used++;
		$ret[$line_counter]['name']=$name;
		$ret[$line_counter]['desc']=$desc;
		$ret[$line_counter]['1aura']=0;
		if($a1==$aura_name)
			$ret[$line_counter]['1aura']++;
		if($a2==$aura_name)
			$ret[$line_counter]['1aura']++;
		if($a3==$aura_name)
			$ret[$line_counter]['1aura']++;
		$line_counter++;
		?>
		<tr>
			<td><?=$line_counter?></td>
			<td><?=$name?></td>
			<td><?=$desc?></td>
			<td><?=$works?></td>
		</tr>
		<?php
	}
		?>
	</table>
	<?php
	echo "from these $might_work might work and ".($line_counter-$might_work)." might not and $not_used might not be used<br>";
	$ret['lines']=$line_counter;
	return $ret;
}
?>