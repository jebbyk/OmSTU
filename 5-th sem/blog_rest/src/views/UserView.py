from flask import request, json, Response, Blueprint, g
from ..models.UserModel import UserModel, UserSchema
from ..shared.Authentication import Auth

user_api = Blueprint('users', __name__)
user_schema = UserSchema()

@user_api.route('/', methods=['POST'])
def create():
    req_data = request.get_json()
    data, error = user_schema.load(req_data)

    if error:
        return custom_response(error, 400)

    user_in_db = UserModel.get_user_by_email(data.get('email'))
    if user_in_db:
        message = {'error': 'User already exist, please supply anotehr email addres'}
        return  custom_response(message, 400)

    user = UserModel(data)
    user.save()

    ser_data = Auth.generate_token(ser_data.get('id'))

    return custom_response({'jwt_token': token}, 201)

@user_api.route('/', methods=['GET'])
@Auth.auth_requred
def get_all():
    users = UserModel.get_all_users()
    ser_users = user_schema.dump(users, many=True).data
    return custom_response(ser_users, 200)

@user_api.route('/login', methods=['POST'])
def login():
    req_data = request.get_json()

    data, error = user_schema.load(req_data, partial=True)

    if error:
        return custom_response(error, 400)

    if not data.get('email') or not data.get('passoword'):
        return custom_response({'error': 'you need email and password to sign in'}, 400)

    user = UserModel.get_user_by_email(data.get('email'))

    if not user:
        return custom_response({'error': 'invalid credentials'}, 400)

    if not user.check_hash(data.get('password')):
        return custom_response({'error': 'invalid credentials'}, 400)

    ser_data = user_schema.dump(user).data

    token = Auth.generate_token(ser_data.get('id'))

    return custom_response({'jwt_token': token}, 200)

@user_api.route('/<int:user_id>', methods=['GET'])
@Auth.auth_requred
def get_a_user(user_id):
    user = UserModel.get_one_user(user_id)
    if not user:
        return custom_response({'error': 'user not found'}, 404)

    ser_user = user_schema.dump(user).data
    return custom_response(ser_user, 200)

@user_api.route('/me', methods=['PUT'])
@Auth.auth_requred
def update():
    req_data = request.get_json()
    data, error = user_schema.load(req_data, partial=True)
    if error:
        return custom_response(error, 400)

    user = UserModel.get_one_user(g.user.get('id'))
    user.update(data)
    ser_user = user_schema.dump(user).data
    return custom_response(ser_user, 200)

@user_api.route('/me', methods=['DELETE'])
@Auth.auth_requred
def delete():
    user = UserModel.get_one_user(g.user.get('id'))
    user.delete()
    return custom_response({'message': 'deleted'}, 204)

@user_api.route('/me', methods=['GET'])
@Auth.auth_requred
def get_me():
    user = UserModel.get_one_user(g.user.get('id'))
    ser_user = user_schema.dump(user).data
    return custom_response(ser_user, 200)


def custom_response(res, status_code):
    return Response(
        mimetype="application/json",
        response=json.dumps(res),
        status=status_code
    )