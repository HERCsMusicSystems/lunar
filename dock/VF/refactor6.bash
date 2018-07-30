Echo $1
sed -i '' 's/\[vco algo 1 2\]/\[vco algo 6 5\]/g' $1
sed -i '' 's/\[vco algo 1 3\]/\[vco algo 6 4\]/g' $1
sed -i '' 's/\[vco algo 1 4\]/\[vco algo 6 3\]/g' $1
sed -i '' 's/\[vco algo 1 5\]/\[vco algo 6 2\]/g' $1
sed -i '' 's/\[vco algo 1 6\]/\[vco algo 6 1\]/g' $1
sed -i '' 's/\[vco algo 2 3\]/\[vco algo 5 4\]/g' $1
sed -i '' 's/\[vco algo 2 4\]/\[vco algo 5 3\]/g' $1
sed -i '' 's/\[vco algo 2 5\]/\[vco algo 5 2\]/g' $1
sed -i '' 's/\[vco algo 2 6\]/\[vco algo 5 1\]/g' $1
sed -i '' 's/\[vco algo 3 4\]/\[vco algo 4 3\]/g' $1
sed -i '' 's/\[vco algo 3 5\]/\[vco algo 4 2\]/g' $1
sed -i '' 's/\[vco algo 3 6\]/\[vco algo 4 1\]/g' $1
sed -i '' 's/\[vco algo 4 5\]/\[vco algo 3 2\]/g' $1
sed -i '' 's/\[vco algo 4 6\]/\[vco algo 3 1\]/g' $1
sed -i '' 's/\[vco algo 5 6\]/\[vco algo 2 1\]/g' $1
sed -i '' 's/\[vco 1/\[vco @1/g' $1
sed -i '' 's/\[vco 2/\[vco @2/g' $1
sed -i '' 's/\[vco 3/\[vco @3/g' $1
sed -i '' 's/\[vco 4/\[vco @4/g' $1
sed -i '' 's/\[vco 5/\[vco @5/g' $1
sed -i '' 's/\[vco 6/\[vco @6/g' $1
sed -i '' 's/\[vco @1/\[vco 6/g' $1
sed -i '' 's/\[vco @2/\[vco 5/g' $1
sed -i '' 's/\[vco @3/\[vco 4/g' $1
sed -i '' 's/\[vco @4/\[vco 3/g' $1
sed -i '' 's/\[vco @5/\[vco 2/g' $1
sed -i '' 's/\[vco @6/\[vco 1/g' $1
