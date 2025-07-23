# setp 0 - setup vars

if [[ -f build/auth.json ]]; then
  echo "auth.json already exists"
  exit 0
fi

name="root"
password="admin"
email="$name@email.com"
url="127.0.0.1:2283"

# step 1 - create admin
create_res=$(curl -sL "$url/api/auth/admin-sign-up" \
-H 'Content-Type: application/json' \
-H 'Accept: application/json' \
--data-raw "{
    \"email\": \"$email\",
    \"name\": \"$name\",
    \"password\": \"$password\"
}")

# step 2 - login with username and password to get bearer token
login_res=$(curl -sL "$url/api/auth/login" \
-H 'Content-Type: application/json' \
-H 'Accept: application/json' \
--data-raw "{
    \"email\": \"$email\",
    \"password\": \"$password\"
}")

token=$(echo "$login_res" | jq -r '.accessToken')

# step 3 - create and api key
key_res=$(curl -sL "$url/api/api-keys" \
-H 'Content-Type: application/json' \
-H 'Accept: application/json' \
-H "Authorization: Bearer $token" \
-d '{
  "name": "string",
  "permissions": [
    "all"
  ]
}')

key=$(echo "$key_res" | jq -r '.secret')

# step 4 - write to file
echo "{ 
  \"server\":\"$url\", 
  \"secret\":\"$key\" 
}" > build/auth.json
