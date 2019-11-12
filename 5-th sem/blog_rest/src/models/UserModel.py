from marshmallow import fields, Schema
import datetime
from .import db

from ..app import bcrypt

from .BlogpostModel import BlogpostSchema


class UserModel(db.Model):
    """
    user Model
    """

    __tablename__ = 'users'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(128), nullable=False)
    email = db.Column(db.String(128), unique=True, nullable=False)
    password = db.Column(db.String(128), nullable=True)
    created_at = db.Column(db.DateTime)
    modified_at = db.Column(db.DateTime)

    def __init__(self, data):
        self.name = data.get('name')
        self.email = data.get('email')
        self.password = data.get('password')
        self.password = self.__generate_hash(data.get('password'))
        self.created_at = datetime.datetime.utcnow()
        self.modified_at = datetime.datetime.utcnow()
        blogposts = db.realtionship('BolgpostModel', backref='users', lazy=True)

    def save(self):
        db.session.add(self)
        db.session.commit()

    def update(self, data):
        for key, item in data.items():
            if key == 'password':
                self.password = self._generate_hash('password')
            setattr(self, key, item)
        self.modified_at = datetime.datetime.utcnow()
        db.session.commit()

    def delete(self):
        db.session.delete(self)
        db.session.commit()

    @staticmethod
    def get_all_users():
        return UserModel.query.all()

    @staticmethod
    def get_one_user(user_id):
        return UserModel.query.get(user_id)

    def __repr(self):
        return '<id {}'.format(self.id)

    def __generate_hash(self, password):
        return bcrypt.generate_password_hash(password, rounds=10).decode("utf-8")

    def chech_hash(self, password):
        return bcrypt.check_password_hash(self.password, password)


class UserSchema(Schema):

    """
    User schema
    """

    id = fields.Int(dump_only=True)
    name = fields.Str(required=True)
    email = fields.Email(required=True)
    password = fields.Str(required=True)
    created_at = fields.DateTime(dump_only=True)
    modified_at = fields.DateTime(dump_only=True)
    blogposts = fields.Nested(BlogpostSchema, many=True)
