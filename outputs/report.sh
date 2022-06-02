#!/bin/bash

echo "|depth|games |score/game|% 65536|% 32768|% 16384|% 8192 |moves/s|seconds/game|"
echo "|-----|------|----------|-------|-------|-------|-------|-------|------------|"
for depth in {3..8}; do
  printf "|   $depth "

  output=2048b.d$depth
  num_games=$(grep max $output | wc -l)
  printf "| $num_games "

  sum_score=$(awk -F ' ' '$1 ~ /score/ {sum += $2} END {print sum}' $output)
  avg_score=$(echo "scale=3;$sum_score/$num_games" | bc)
  printf "|   %.0f " $avg_score

  running_count=0
  max=65536
  while [[ $max -ge 8192 ]]; do
    count=$(grep "max $max" $output | wc -l)
    running_count=$((running_count+count))
    percent=$(echo "scale=3;$running_count*100/$num_games" | bc)
    printf "| %5.1f " $percent
    max=$((max >> 1))
  done

  sum_moves=$(awk -F ' ' '$3 ~ /moves/ {sum += $4} END {print sum}' $output)
  sum_seconds=$(awk -F ' ' '$5 ~ /seconds/ {sum += $6} END {printf "%.1f", sum}' $output)
  moves_per_second=$(echo "scale=3;$sum_moves/$sum_seconds" | bc)
  printf "| %5.0f " $moves_per_second

  seconds_per_game=$(echo "scale=3;$sum_seconds/$num_games" | bc)
  printf "|     %6.0f |\n" $seconds_per_game
done
