from flask import Flask

from .config import app_config
from .models import db, bcrypt

def create_app(env_name):
    """
    Create app
    :param env_name:
    :return:
    """
    #initialization
    app = Flask(__name__)

    app.config.from_object(app_config[env_name])
    bcrypt.init_app(app)
    db.init_app(app)

    @app.route('/', methods = ['GET'])
    def index():
        """
        endpoint
        :return:
        """

        return 'Congratulations'
    return app