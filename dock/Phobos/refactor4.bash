sed -i '' 's/\[vco algo 1 2\]/\[vco algo 4 3\]/g' $1
sed -i '' 's/\[vco algo 1 3\]/\[vco algo 4 2\]/g' $1
sed -i '' 's/\[vco algo 1 4\]/\[vco algo 4 1\]/g' $1
sed -i '' 's/\[vco algo 2 3\]/\[vco algo 3 2\]/g' $1
sed -i '' 's/\[vco algo 2 4\]/\[vco algo 3 1\]/g' $1
sed -i '' 's/\[vco algo 3 4\]/\[vco algo 2 1\]/g' $1
sed -i '' 's/\[vco 1/\[vco @1/g' $1
sed -i '' 's/\[vco 2/\[vco @2/g' $1
sed -i '' 's/\[vco 3/\[vco @3/g' $1
sed -i '' 's/\[vco 4/\[vco @4/g' $1
sed -i '' 's/\[vco @1/\[vco 4/g' $1
sed -i '' 's/\[vco @2/\[vco 3/g' $1
sed -i '' 's/\[vco @3/\[vco 2/g' $1
sed -i '' 's/\[vco @4/\[vco 1/g' $1
